#pragma once

#include <string>
#include <cstdint>

struct AccessTokenData {
    std::int64_t id;
    std::int64_t family_id;
    std::int64_t ttl;
};
