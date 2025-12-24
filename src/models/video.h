#pragma once

#include <chrono>
#include <cstdint>
#include <optional>
#include <string>

struct Video {
    std::int64_t id;
    std::int64_t family_id;

    double file_size_mb = 0.0;

    bool is_active = true;

    std::string name;
    std::string hash;
    std::string mime_type;
    std::string created_at;
    std::string updated_at;

    std::optional<std::string> description;
    std::optional<double>      resolution_width_px;
    std::optional<double>      resolution_height_px;
    std::optional<double>      duration_sec;
    std::optional<double>      frame_rate;
};

struct VideoListResult {
    std::size_t        count;
    std::vector<Video> list;
};