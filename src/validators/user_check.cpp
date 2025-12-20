#include "user_check.h"

namespace validate {
    bool user_check(const rapidjson::Document& body, std::string& error) {
        if (!body.HasMember("key")) {
            error = "Missing 'key'";
            return false;
        }
        if (!body["key"].IsString()) {
            error = "'key' must be a string";
            return false;
        }
        return true;
    }
} // namespace validate