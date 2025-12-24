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
