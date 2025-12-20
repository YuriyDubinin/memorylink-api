#pragma once

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <string>
#include <utils/http_response.h>

namespace validate {
    bool user_check(const rapidjson::Document& body, std::string& error_msg);
}