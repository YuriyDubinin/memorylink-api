#pragma once

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <string>

struct ApiResponse {
    std::string type;
    int         http_code;
    std::string message;
};