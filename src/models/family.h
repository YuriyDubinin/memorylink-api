#pragma once

#include <chrono>
#include <cstdint>
#include <optional>
#include <string>

enum class FamilyStatus { ACTIVE, FROZEN, DELETED };

struct Family {
    std::int64_t                          id;
    std::string                           name;
    std::optional<std::string>            description;
    std::int64_t                          owner_id;
    std::int64_t                          tariff_id;
    FamilyStatus                          status = FamilyStatus::ACTIVE;
    std::optional<std::string>            status_description;
    int                                   storage_limit_gb = 5;
    int                                   storage_used_gb  = 0;
    std::chrono::system_clock::time_point created_at       = std::chrono::system_clock::now();
    std::chrono::system_clock::time_point updated_at       = std::chrono::system_clock::now();
};
