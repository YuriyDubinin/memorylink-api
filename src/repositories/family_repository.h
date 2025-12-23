#pragma once

#include <optional>
#include <string>

#include "models/family.h"

class FamilyRepository {
public:
    virtual ~FamilyRepository() {}

    virtual std::optional<Family> GetById(std::int64_t user_id) = 0;
};