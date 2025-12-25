#pragma once

#include <string>

#include <httplib/httplib.h>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include <models/api_response.h>
#include <utils/http_response.h>
#include <validators/access_token.h>
#include <validators/json.h>

namespace validate {
    bool file_request(const httplib::Request& req, ApiResponse& api_response);
}