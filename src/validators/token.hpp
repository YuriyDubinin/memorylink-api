#pragma once

#include <httplib/httplib.h>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <string>
#include <utils/http_response.h>

namespace validate {
    bool token(const rapidjson::Document& body_json, httplib::Response& res,
               std::string& error_msg);
}