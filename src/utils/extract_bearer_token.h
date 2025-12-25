#pragma once
#include <optional>
#include <string>

#include <httplib/httplib.h>

namespace utils {
    std::optional<std::string> extract_bearer_token(const httplib::Request& req);

}