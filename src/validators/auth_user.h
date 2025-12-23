#pragma once

#include <string>

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include <models/api_response.h>
#include <utils/http_response.h>
#include <utils/password.h>

namespace validate {
    bool auth_user(const rapidjson::Document& body_json, ApiResponse& api_response);
}