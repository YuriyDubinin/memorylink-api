#pragma once

#include <optional>
#include <string>

#include "models/video.h"

class VideoRepository {
public:
    virtual ~VideoRepository() {}

    virtual std::optional<Video> GetById(std::int64_t video_id) = 0;
};