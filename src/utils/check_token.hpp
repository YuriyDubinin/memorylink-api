#pragma once

#include <string>
#include <httplib/httplib.h>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <utils/http_response.h>

namespace utils::guard {
    bool check_token(const rapidjson::Document& json, httplib::Response& res);
}