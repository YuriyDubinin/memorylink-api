#pragma once

#include <string>

#include <httplib/httplib.h>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include <models/api_response.h>

namespace validate {
    bool token(const rapidjson::Document& body_json, ApiResponse& api_response);
}