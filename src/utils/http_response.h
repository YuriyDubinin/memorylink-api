#pragma once

#include <httplib/httplib.h>
#include <rapidjson/document.h>
#include <string>

namespace utils::http_response {
    void send_json_response(httplib::Response& res,
                            const std::string& status,
                            const int& code,
                            const std::string& message,
                            const rapidjson::Value& data = rapidjson::Value(rapidjson::kObjectType));
}
