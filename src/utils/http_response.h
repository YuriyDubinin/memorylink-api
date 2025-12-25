#pragma once

#include <string>

#include <httplib/httplib.h>

#include <models/api_response.h>

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

namespace utils::http_response {
    void send(httplib::Response&      res,
              ApiResponse&            api_response,
              const rapidjson::Value& data = rapidjson::Value(rapidjson::kObjectType));

}
