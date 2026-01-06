#include "pg_photo_repository.h"

PgPhotoRepository::PgPhotoRepository(PostgresConnection& db_conn) : db_(db_conn) {
    db_.prepare("get_photo_by_id",
                "SELECT id, family_id, is_active, file_size_mb, name, hash, mime_type, "
                "description, resolution_width_px, resolution_height_px, created_at, updated_at "
                "FROM photos WHERE id = $1");

    db_.prepare("get_photos_by_family_id",
                "SELECT id, family_id, is_active, file_size_mb, name, hash, mime_type, "
                "description, resolution_width_px, resolution_height_px, created_at, updated_at "
                "FROM photos "
                "WHERE family_id = $1 "
                "ORDER BY id "
                "LIMIT $2 OFFSET $3");

    db_.prepare("count_photos_by_family_id", "SELECT COUNT(*) FROM photos WHERE family_id = $1");
}

std::optional<Photo> PgPhotoRepository::GetById(std::int64_t photo_id) {
    try {
        const pqxx::result res = db_.execute_prepared("get_photo_by_id", photo_id);
        if (res.empty())
            return std::nullopt;

        const auto& row = res[0];
        Photo       photo;

        photo.id           = row["id"].as<std::int64_t>();
        photo.family_id    = row["family_id"].as<std::int64_t>();
        photo.is_active    = row["is_active"].as<bool>();
        photo.file_size_mb = row["file_size_mb"].as<double>();

        photo.name      = row["name"].c_str();
        photo.hash      = row["hash"].c_str();
        photo.mime_type = row["mime_type"].c_str();

        if (!row["description"].is_null())
            photo.description = row["description"].c_str();
        if (!row["resolution_width_px"].is_null())
            photo.resolution_width_px = row["resolution_width_px"].as<double>();
        if (!row["resolution_height_px"].is_null())
            photo.resolution_height_px = row["resolution_height_px"].as<double>();

        if (!row["created_at"].is_null())
            photo.created_at = utils::time::format_pg_timestamp(row["created_at"].c_str());
        if (!row["updated_at"].is_null())
            photo.updated_at = utils::time::format_pg_timestamp(row["updated_at"].c_str());

        return photo;
    } catch (const std::exception& e) {
        std::cout << "PgPhotoRepository::GetById failed: " << e.what() << std::endl;
        throw std::runtime_error(std::string("PgPhotoRepository::GetById failed: ") + e.what());
    }
}

PhotoListResult PgPhotoRepository::GetListByFamilyId(std::int64_t family_id,
                                                     std::size_t  limit,
                                                     std::size_t  offset) {
    try {
        // count
        const pqxx::result count_res = db_.execute_prepared("count_photos_by_family_id", family_id);

        PhotoListResult result;
        result.count = count_res[0][0].as<std::size_t>();

        // list
        const pqxx::result res = db_.execute_prepared("get_photos_by_family_id",
                                                      family_id,
                                                      static_cast<int>(limit),
                                                      static_cast<int>(offset));

        result.list.reserve(res.size());

        for (const auto& row : res) {
            Photo photo;

            photo.id           = row["id"].as<std::int64_t>();
            photo.family_id    = row["family_id"].as<std::int64_t>();
            photo.is_active    = row["is_active"].as<bool>();
            photo.file_size_mb = row["file_size_mb"].as<double>();

            photo.name      = row["name"].c_str();
            photo.hash      = row["hash"].c_str();
            photo.mime_type = row["mime_type"].c_str();

            if (!row["description"].is_null())
                photo.description = row["description"].c_str();
            if (!row["resolution_width_px"].is_null())
                photo.resolution_width_px = row["resolution_width_px"].as<double>();
            if (!row["resolution_height_px"].is_null())
                photo.resolution_height_px = row["resolution_height_px"].as<double>();

            if (!row["created_at"].is_null())
                photo.created_at = utils::time::format_pg_timestamp(row["created_at"].c_str());
            if (!row["updated_at"].is_null())
                photo.updated_at = utils::time::format_pg_timestamp(row["updated_at"].c_str());

            result.list.push_back(std::move(photo));
        }

        return result;
    } catch (const std::exception& e) {
        std::cout << "PgPhotoRepository::GetListByFamilyId failed: " << e.what() << std::endl;
        throw std::runtime_error(std::string("PgPhotoRepository::GetListByFamilyId failed: ") +
                                 e.what());
    }
}

void PgPhotoRepository::InsertListByFamilyId(std::int64_t        family_id,
                                             std::vector<Photo>& photo_list) {
    if (photo_list.empty())
        return;

    try {
        pqxx::work        txn(db_.raw());
        std::stringstream ss;
        ss << "INSERT INTO photos (family_id, name, description, file_size_mb, mime_type, "
              "resolution_width_px, resolution_height_px, hash, is_active) VALUES ";

        for (size_t i = 0; i < photo_list.size(); ++i) {
            const auto& photo = photo_list[i];

            ss << "(" << family_id << ", " << txn.quote(photo.name) << ", "
               << (photo.description ? txn.quote(*photo.description) : "NULL") << ", " << std::fixed
               << std::setprecision(2) << photo.file_size_mb << ", " << txn.quote(photo.mime_type)
               << ", "
               << (photo.resolution_width_px ? std::to_string(*photo.resolution_width_px) : "NULL")
               << ", "
               << (photo.resolution_height_px ? std::to_string(*photo.resolution_height_px)
                                              : "NULL")
               << ", " << txn.quote(photo.hash) << ", " << (photo.is_active ? "true" : "false")
               << ")";

            if (i != photo_list.size() - 1)
                ss << ", ";
        }

        ss << ";";

        txn.exec(ss.str());
        txn.commit();
    } catch (const std::exception& e) {
        std::cerr << "[PgPhotoRepository::InsertListByFamilyId]: failed: " << e.what() << std::endl;
        throw std::runtime_error(
            std::string("[PgPhotoRepository::InsertListByFamilyId]: failed: ") + e.what());
    }
}
