#pragma once

#include <optional>
#include <string>

#include "models/photo.h"

class PhotoRepository {
public:
    virtual ~PhotoRepository() {}

    virtual std::optional<Photo> GetById(std::int64_t user_id) = 0;
};