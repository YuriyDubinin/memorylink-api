#pragma once

#include <string>

#include <models/api_response.h>
#include <utils/http_response.h>
#include <utils/password.h>

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

namespace validate {
    bool auth_user(const rapidjson::Document& body_json, ApiResponse& api_response);
}