#pragma once

#include <string>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

namespace validators {
    bool validate_user_check(const rapidjson::Document& body, std::string& error_msg);
}