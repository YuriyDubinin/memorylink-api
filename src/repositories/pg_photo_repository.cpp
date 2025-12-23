#include "pg_photo_repository.h"

PgPhotoRepository::PgPhotoRepository(PostgresConnection& db_conn) : db_(db_conn) {
    db_.prepare("get_photo_by_id",
                "SELECT id, family_id, is_active, file_size_mb, name, hash, mime_type, "
                "description, resolution_width_px, resolution_height_px, created_at, updated_at "
                "FROM photos WHERE id = $1");
}

std::optional<Photo> PgPhotoRepository::GetById(std::int64_t photo_id) {
    try {
        pqxx::result res = db_.execute_prepared("get_photo_by_id", photo_id);
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
        throw std::runtime_error(std::string("GetById failed: ") + e.what());
    }
}
