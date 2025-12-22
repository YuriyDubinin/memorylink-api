#pragma once

#include <cstdint>
#include <string>

struct AccessTokenData {
    std::int64_t id;
    std::int64_t family_id;
    std::int64_t ttl;
};
