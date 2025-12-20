#pragma once

#include <models/api_response.h>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <string>
#include <utils/http_response.h>

namespace validate {
    bool auth(const rapidjson::Document& body, ApiResponse& api_response);
}