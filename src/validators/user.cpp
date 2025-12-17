#include "user.hpp"

bool ValidateUserCheck(const rapidjson::Document& body, std::string& error) {
    if (!body.HasMember("key")) {
        error = "Missing 'key' field";
        return false;
    }

    if (!body["key"].IsString()) {
        error = "'key' must be a string";
        return false;
    }

    return true;
}
