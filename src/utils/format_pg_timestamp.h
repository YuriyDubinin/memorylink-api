#pragma once

#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <string>

namespace utils::time {
    std::string format_pg_timestamp(const std::string& value);
} // namespace utils::time