#include "pg_user_repository.h"

#include <stdexcept>

PgUserRepository::PgUserRepository(PostgresConnection& db_conn) : db_(db_conn) {
    db_.prepare("get_user_by_id",
                "SELECT id, is_active, email, full_name, password_hash, phone, address, family_id "
                "FROM users WHERE id = $1");
    db_.prepare("get_user_by_email",
                "SELECT id, is_active, email, full_name, password_hash, phone, address, family_id "
                "FROM users WHERE email = $1");
}

std::optional<User> PgUserRepository::GetById(std::int64_t user_id) {
    try {
        pqxx::result res = db_.execute_prepared("get_user_by_id", user_id);
        if (res.empty())
            return std::nullopt;

        const auto& row = res[0];

        User user;
        user.id            = row["id"].as<std::int64_t>();
        user.is_active     = row["is_active"].as<bool>();
        user.full_name     = row["full_name"].c_str();
        user.email         = row["email"].c_str();
        user.password_hash = row["password_hash"].c_str();
        if (!row["phone"].is_null())
            user.phone = row["phone"].c_str();
        if (!row["address"].is_null())
            user.address = row["address"].c_str();
        user.family_id = row["family_id"].as<std::int64_t>();

        return user;
    } catch (const std::exception& e) {
        throw std::runtime_error(std::string("GetById failed: ") + e.what());
    }
}

std::optional<User> PgUserRepository::GetByEmail(const std::string& email) {
    try {
        pqxx::result res = db_.execute_prepared("get_user_by_email", email);
        if (res.empty())
            return std::nullopt;

        const auto& row = res[0];

        User user;
        user.id            = row["id"].as<std::int64_t>();
        user.is_active     = row["is_active"].as<bool>();
        user.full_name     = row["full_name"].c_str();
        user.email         = row["email"].c_str();
        user.password_hash = row["password_hash"].c_str();
        if (!row["phone"].is_null())
            user.phone = row["phone"].c_str();
        if (!row["address"].is_null())
            user.address = row["address"].c_str();
        user.family_id = row["family_id"].as<std::int64_t>();

        return user;
    } catch (const std::exception& e) {
        throw std::runtime_error(std::string("GetByEmail failed: ") + e.what());
    }
}
