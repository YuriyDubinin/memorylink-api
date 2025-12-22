#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <chrono>

struct User {
    std::int64_t id;
    bool is_active;

    std::string full_name;
    std::string email;
    std::string password_hash;

    std::optional<std::string> phone;
    std::optional<std::string> address;

    std::chrono::system_clock::time_point created_at;
    std::chrono::system_clock::time_point updated_at;

    std::int64_t family_id;
};
