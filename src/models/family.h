#pragma once

#include <chrono>
#include <cstdint>
#include <optional>
#include <string>

enum class FamilyStatus { ACTIVE, FROZEN, DELETED };

struct Family {
    std::int64_t id;
    std::int64_t owner_id;
    std::int64_t tariff_id;

    int storage_limit_mb = 5210;
    int storage_used_mb  = 0;

    std::string name;
    std::string created_at;
    std::string updated_at;

    std::optional<std::string> description;
    std::optional<std::string> status_description;
    std::optional<std::string> avatar;

    FamilyStatus status = FamilyStatus::ACTIVE;
};
