#pragma once

#include <chrono>
#include <string>

#include <httplib/httplib.h>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include <config/config_manager.h>
#include <models/access_token_data.h>
#include <models/api_response.h>
#include <utils/access_token_crypto.h>

namespace validate {
    bool access_token(const httplib::Request& req, ApiResponse& api_response);
}