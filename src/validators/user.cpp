#include "user.hpp"

namespace validators {
    bool validate_user_check(const rapidjson::Document& body, std::string& error) {
        if (!body.HasMember("key")) { error = "Missing 'key'"; return false; }
        if (!body["key"].IsString()) { error = "'key' must be a string"; return false; }
        return true;
    }
}