#pragma once

#include <optional>
#include <string>

#include "models/user.h"

class UserRepository {
public:
    virtual ~UserRepository() {}

    virtual std::optional<User> GetById(std::int64_t user_id)        = 0;
    virtual std::optional<User> GetByEmail(const std::string& email) = 0;
};