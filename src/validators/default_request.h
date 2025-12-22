#pragma once

#include <string>

#include <httplib/httplib.h>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include <models/api_response.h>
#include <utils/http_response.h>
#include <validators/json.h>
#include <validators/token.h>

namespace validate {
    bool default_request(const httplib::Request& req,
                         httplib::Response&      res,
                         rapidjson::Document&    body_json,
                         ApiResponse&            api_response);
}