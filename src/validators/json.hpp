#pragma once

#include <httplib/httplib.h>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <string>
#include <utils/http_response.h>

namespace validate {
    bool json(const httplib::Request& req, httplib::Response& res, rapidjson::Document& body,
              std::string& error_msg);
}