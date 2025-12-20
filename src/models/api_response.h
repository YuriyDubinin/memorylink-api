#pragma once

#include <string>

struct ApiResponse {
    std::string type;
    int http_code;
    std::string message;
};