#pragma once

#include <chrono>
#include <cstdint>
#include <optional>
#include <string>

struct Photo {
    std::int64_t id;
    std::int64_t family_id;

    bool is_active = true;

    double file_size_mb = 0.0;

    std::string name;
    std::string hash;
    std::string mime_type;
    std::string created_at;
    std::string updated_at;

    std::optional<std::string> description;
    std::optional<double>      resolution_width_px;
    std::optional<double>      resolution_height_px;
};

struct PhotoListResult {
    std::size_t        count;
    std::vector<Photo> list;
};
