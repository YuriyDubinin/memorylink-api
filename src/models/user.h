#pragma once

#include <chrono>
#include <cstdint>
#include <optional>
#include <string>

enum class UserStatus { ACTIVE, DELETED };
enum class UserRole { OWNER, ADMIN, READER };

struct User {
    std::int64_t id;
    std::int64_t family_id;

    std::string full_name;
    std::string email;
    std::string password_hash;
    std::string created_at;
    std::string updated_at;

    std::optional<std::string> phone;
    std::optional<std::string> address;
    std::optional<std::string> avatar;

    UserStatus status = UserStatus::ACTIVE;
    UserRole   role   = UserRole::READER;
};
