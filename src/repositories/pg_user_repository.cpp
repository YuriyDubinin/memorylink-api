#include "pg_user_repository.h"

#include <stdexcept>

PgUserRepository::PgUserRepository(PostgresConnection& db_conn) : db_(db_conn) {
    db_.prepare("get_user_by_id",
                "SELECT id, email, full_name, password_hash, phone, address, family_id, "
                "created_at, updated_at, avatar, status, role "
                "FROM users WHERE id = $1");
    db_.prepare("get_user_by_email",
                "SELECT id, email, full_name, password_hash, phone, address, family_id, "
                "created_at, updated_at, avatar, status, role "
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
        user.full_name     = row["full_name"].c_str();
        user.email         = row["email"].c_str();
        user.password_hash = row["password_hash"].c_str();
        if (!row["phone"].is_null())
            user.phone = row["phone"].c_str();
        if (!row["address"].is_null())
            user.address = row["address"].c_str();
        user.family_id = row["family_id"].as<std::int64_t>();

        if (!row["created_at"].is_null())
            user.created_at = utils::time::format_pg_timestamp(row["created_at"].c_str());
        if (!row["updated_at"].is_null())
            user.updated_at = utils::time::format_pg_timestamp(row["updated_at"].c_str());

        if (!row["status"].is_null())
            user.status = ParseStatus(row["status"].c_str());
        if (!row["role"].is_null())
            user.role = ParseRole(row["role"].c_str());

        return user;
    } catch (const std::exception& e) {
        std::cout << "PgUserRepository::GetById failed: " << e.what() << std::endl;
        throw std::runtime_error(std::string("PgUserRepository::GetById failed: ") + e.what());
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
        user.full_name     = row["full_name"].c_str();
        user.email         = row["email"].c_str();
        user.password_hash = row["password_hash"].c_str();
        if (!row["phone"].is_null())
            user.phone = row["phone"].c_str();
        if (!row["address"].is_null())
            user.address = row["address"].c_str();
        user.family_id = row["family_id"].as<std::int64_t>();
        if (!row["avatar"].is_null())
            user.avatar = row["avatar"].c_str();

        if (!row["created_at"].is_null())
            user.created_at = utils::time::format_pg_timestamp(row["created_at"].c_str());
        if (!row["updated_at"].is_null())
            user.updated_at = utils::time::format_pg_timestamp(row["updated_at"].c_str());

        user.status = ParseStatus(row["status"].c_str());
        user.role   = ParseRole(row["role"].c_str());

        return user;
    } catch (const std::exception& e) {
        std::cout << "PgUserRepository::GetByEmail failed: " << e.what() << std::endl;
        throw std::runtime_error(std::string("PgUserRepository::GetByEmail failed: ") + e.what());
    }
}

UserStatus PgUserRepository::ParseStatus(const std::string& status_str) {
    if (status_str == "ACTIVE")
        return UserStatus::ACTIVE;
    if (status_str == "DELETED")
        return UserStatus::DELETED;

    std::cout << "PgUserRepository::ParseStatus: Unknown user status: " << status_str << std::endl;
    throw std::runtime_error("PgUserRepository::ParseStatus: Unknown user status: " + status_str);
}

UserRole PgUserRepository::ParseRole(const std::string& status_str) {
    if (status_str == "OWNER")
        return UserRole::OWNER;
    if (status_str == "ADMIN")
        return UserRole::ADMIN;
    if (status_str == "READER")
        return UserRole::READER;

    std::cout << "PgUserRepository::ParseStatus: Unknown user role: " << status_str << std::endl;
    throw std::runtime_error("PgUserRepository::ParseStatus: Unknown user role: " + status_str);
}

std::string PgUserRepository::FormatStatusToString(UserStatus status) {
    switch (status) {
        case UserStatus::ACTIVE:
            return "ACTIVE";
        case UserStatus::DELETED:
            return "DELETED";
        default:
            throw std::runtime_error("FormatStatusToString: Unknown UserStatus enum value");
    }
}

std::string PgUserRepository::FormatRoleToString(UserRole role) {
    switch (role) {
        case UserRole::OWNER:
            return "OWNER";
        case UserRole::ADMIN:
            return "ADMIN";
        case UserRole::READER:
            return "READER";
        default:
            throw std::runtime_error("FormatRoleToString: Unknown UserRole enum value");
    }
}