#include "pg_family_repository.h"

PgFamilyRepository::PgFamilyRepository(PostgresConnection& db_conn) : db_(db_conn) {
    db_.prepare("get_family_by_id",
                "SELECT id, name, description, owner_id, tariff_id, status, status_description, "
                "storage_limit_mb, storage_used_mb, avatar, created_at, updated_at "
                "FROM families WHERE id = $1");
}

std::optional<Family> PgFamilyRepository::GetById(std::int64_t family_id) {
    try {
        pqxx::result res = db_.execute_prepared("get_family_by_id", family_id);
        if (res.empty())
            return std::nullopt;

        const auto& row = res[0];
        Family      family;

        family.id   = row["id"].as<std::int64_t>();
        family.name = row["name"].c_str();
        if (!row["description"].is_null())
            family.description = row["description"].c_str();
        family.owner_id  = row["owner_id"].as<std::int64_t>();
        family.tariff_id = row["tariff_id"].as<std::int64_t>();

        family.status = ParseStatus_(row["status"].c_str());
        if (!row["status_description"].is_null())
            family.status_description = row["status_description"].c_str();
        if (!row["avatar"].is_null())
            family.avatar = row["avatar"].c_str();

        family.storage_limit_mb = row["storage_limit_mb"].as<double>();
        family.storage_used_mb  = row["storage_used_mb"].as<double>();

        if (!row["created_at"].is_null())
            family.created_at = utils::time::format_pg_timestamp(row["created_at"].c_str());
        if (!row["updated_at"].is_null())
            family.updated_at = utils::time::format_pg_timestamp(row["updated_at"].c_str());

        return family;
    } catch (const std::exception& e) {
        throw std::runtime_error(std::string("GetById failed: ") + e.what());
    }
}

FamilyStatus PgFamilyRepository::ParseStatus_(const std::string& status_str) {
    if (status_str == "ACTIVE")
        return FamilyStatus::ACTIVE;
    if (status_str == "FROZEN")
        return FamilyStatus::FROZEN;
    if (status_str == "DELETED")
        return FamilyStatus::DELETED;
    throw std::runtime_error("Unknown family status: " + status_str);
}
