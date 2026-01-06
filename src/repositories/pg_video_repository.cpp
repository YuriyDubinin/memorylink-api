#include "pg_video_repository.h"

PgVideoRepository::PgVideoRepository(PostgresConnection& db_conn) : db_(db_conn) {
    db_.prepare("get_video_by_id",
                "SELECT id, family_id, is_active, file_size_mb, name, hash, mime_type, "
                "description, resolution_width_px, resolution_height_px, duration_sec, frame_rate, "
                "created_at, updated_at "
                "FROM videos WHERE id = $1");

    db_.prepare("get_videos_by_family_id",
                "SELECT id, family_id, is_active, file_size_mb, name, hash, mime_type, "
                "description, resolution_width_px, resolution_height_px, duration_sec, frame_rate, "
                "created_at, updated_at "
                "FROM videos "
                "WHERE family_id = $1 "
                "ORDER BY created_at DESC "
                "LIMIT $2 OFFSET $3");

    db_.prepare("count_videos_by_family_id", "SELECT COUNT(*) FROM videos WHERE family_id = $1");
}

std::optional<Video> PgVideoRepository::GetById(std::int64_t video_id) {
    try {
        const pqxx::result res = db_.execute_prepared("get_video_by_id", video_id);
        if (res.empty())
            return std::nullopt;

        const auto& row = res[0];
        Video       video;

        video.id           = row["id"].as<std::int64_t>();
        video.family_id    = row["family_id"].as<std::int64_t>();
        video.is_active    = row["is_active"].as<bool>();
        video.file_size_mb = row["file_size_mb"].as<double>();

        video.name      = row["name"].c_str();
        video.hash      = row["hash"].c_str();
        video.mime_type = row["mime_type"].c_str();

        if (!row["description"].is_null())
            video.description = row["description"].c_str();

        if (!row["resolution_width_px"].is_null())
            video.resolution_width_px = row["resolution_width_px"].as<double>();

        if (!row["resolution_height_px"].is_null())
            video.resolution_height_px = row["resolution_height_px"].as<double>();

        if (!row["duration_sec"].is_null())
            video.duration_sec = row["duration_sec"].as<double>();

        if (!row["frame_rate"].is_null())
            video.frame_rate = row["frame_rate"].as<double>();

        if (!row["created_at"].is_null())
            video.created_at = utils::time::format_pg_timestamp(row["created_at"].c_str());

        if (!row["updated_at"].is_null())
            video.updated_at = utils::time::format_pg_timestamp(row["updated_at"].c_str());

        return video;
    } catch (const std::exception& e) {
        std::cout << "PgVideoRepository::GetById failed: " << e.what() << std::endl;
        throw std::runtime_error(std::string("GetById failed: ") + e.what());
    }
}

VideoListResult PgVideoRepository::GetListByFamilyId(std::int64_t family_id,
                                                     std::size_t  limit,
                                                     std::size_t  offset) {
    try {
        // count
        const pqxx::result count_res = db_.execute_prepared("count_videos_by_family_id", family_id);

        VideoListResult result;
        result.count = count_res[0][0].as<std::size_t>();

        // list
        const pqxx::result res = db_.execute_prepared("get_videos_by_family_id",
                                                      family_id,
                                                      static_cast<int>(limit),
                                                      static_cast<int>(offset));

        result.list.reserve(res.size());

        for (const auto& row : res) {
            Video video;

            video.id        = row["id"].as<std::int64_t>();
            video.family_id = row["family_id"].as<std::int64_t>();
            video.is_active = row["is_active"].as<bool>();

            video.file_size_mb = row["file_size_mb"].as<double>();

            video.name      = row["name"].c_str();
            video.hash      = row["hash"].c_str();
            video.mime_type = row["mime_type"].c_str();

            if (!row["description"].is_null())
                video.description = row["description"].c_str();

            if (!row["resolution_width_px"].is_null())
                video.resolution_width_px = row["resolution_width_px"].as<double>();

            if (!row["resolution_height_px"].is_null())
                video.resolution_height_px = row["resolution_height_px"].as<double>();

            if (!row["duration_sec"].is_null())
                video.duration_sec = row["duration_sec"].as<double>();

            if (!row["frame_rate"].is_null())
                video.frame_rate = row["frame_rate"].as<double>();

            if (!row["created_at"].is_null())
                video.created_at = utils::time::format_pg_timestamp(row["created_at"].c_str());

            if (!row["updated_at"].is_null())
                video.updated_at = utils::time::format_pg_timestamp(row["updated_at"].c_str());

            result.list.push_back(std::move(video));
        }

        return result;
    } catch (const std::exception& e) {
        std::cout << "PgVideoRepository::GetListByFamilyId failed: " << e.what() << std::endl;

        throw std::runtime_error(std::string("PgVideoRepository::GetListByFamilyId failed: ") +
                                 e.what());
    }
}

void PgVideoRepository::InsertListByFamilyId(std::int64_t        family_id,
                                             std::vector<Video>& video_list) {
    if (video_list.empty())
        return;

    try {
        pqxx::work        txn(db_.raw());
        std::stringstream ss;
        ss << "INSERT INTO videos (family_id, name, description, file_size_mb, mime_type, "
              "resolution_width_px, resolution_height_px, duration_sec, frame_rate, hash, "
              "is_active) VALUES ";

        for (size_t i = 0; i < video_list.size(); ++i) {
            const auto& video = video_list[i];

            ss << "(" << family_id << ", " << txn.quote(video.name) << ", "
               << (video.description ? txn.quote(*video.description) : "NULL") << ", " << std::fixed
               << std::setprecision(2) << video.file_size_mb << ", " << txn.quote(video.mime_type)
               << ", "
               << (video.resolution_width_px ? std::to_string(*video.resolution_width_px) : "NULL")
               << ", "
               << (video.resolution_height_px ? std::to_string(*video.resolution_height_px)
                                              : "NULL")
               << ", " << (video.duration_sec ? std::to_string(*video.duration_sec) : "NULL")
               << ", " << (video.frame_rate ? std::to_string(*video.frame_rate) : "NULL") << ", "
               << txn.quote(video.hash) << ", " << (video.is_active ? "true" : "false") << ")";

            if (i != video_list.size() - 1)
                ss << ", ";
        }

        ss << ";";

        txn.exec(ss.str());
        txn.commit();
    } catch (const std::exception& e) {
        std::cerr << "[PgVideoRepository::InsertListByFamilyId] failed: " << e.what() << std::endl;
        throw std::runtime_error(std::string("[PgVideoRepository::InsertListByFamilyId] failed: ") +
                                 e.what());
    }
}
