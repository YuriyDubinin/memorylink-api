#pragma once

#include <string>

#include <httplib/httplib.h>

#include <models/api_response.h>
#include <validators/access_token.h>
#include <validators/json.h>

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

namespace validate {
    bool json_request(const httplib::Request& req,
                      httplib::Response&      res,
                      rapidjson::Document&    body_json,
                      ApiResponse&            api_response);
}