#pragma once

#include <httplib/httplib.h>
#include <models/api_response.h>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <string>
#include <utils/http_response.h>
#include <validators/json.h>
#include <validators/token.h>

namespace validate {
    bool execute(const httplib::Request& req,
                 httplib::Response&      res,
                 rapidjson::Document&    body_json,
                 ApiResponse&            api_response);
}