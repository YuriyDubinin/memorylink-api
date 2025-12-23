#include "format_pg_timestamp.h"

namespace utils::time {
    std::string format_pg_timestamp(const std::string& value) {
        if (value.size() >= 19) {
            return value.substr(0, 19);
        }
        return value;
    }
} // namespace utils::time
