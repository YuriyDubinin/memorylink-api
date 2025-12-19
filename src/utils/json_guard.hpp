#pragma once

#include <string>
#include <httplib/httplib.h>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <utils/http_response.h>

namespace utils::guard {
    bool parse_json(
        const httplib::Request& req,
        httplib::Response& res,
        rapidjson::Document& out_doc);
}