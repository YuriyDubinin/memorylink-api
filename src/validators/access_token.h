#pragma once

#include <string>
#include <chrono>

#include <httplib/httplib.h>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include <models/api_response.h>
#include <models/access_token_data.h>
#include <utils/access_token_crypto.h>
#include <config/config_manager.h>

namespace validate {
    bool access_token(const rapidjson::Document& body_json, ApiResponse& api_response);
}