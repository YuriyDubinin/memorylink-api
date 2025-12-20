#pragma once

#include <httplib/httplib.h>
#include <models/api_response.h>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <string>

namespace validate {
    bool
    json(const httplib::Request& req, rapidjson::Document& body_json, ApiResponse& api_response);
}