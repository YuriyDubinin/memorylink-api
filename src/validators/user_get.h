#pragma once

#include <string>

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include <models/api_response.h>
#include <utils/http_response.h>

namespace validate {
    bool user_get(const rapidjson::Document& body, ApiResponse& api_response);
}