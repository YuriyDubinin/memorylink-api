#pragma once

#include <string>

struct AccessToken {
    std::int64_t id;
    std::string  full_name;
    std::string  password_hash;
    std::int64_t family_id;
    std::int64_t ttl;
};
