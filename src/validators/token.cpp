#include "token.hpp"

namespace utils::guard {
    constexpr std::string EXPECTED_TOKEN = "SECRET_TOKEN";

    bool token(const rapidjson::Document& body_json, httplib::Response& res,
               std::string& error_msg) {
        if (!body_json.HasMember("token")) {
            error_msg = "Missing 'token'";
            return false;
        }

        if (!body_json["token"].IsString()) {
            error_msg = "'token' must be a string";
            return false;
        }

        const std::string token = body_json["token"].GetString();
        if (token != EXPECTED_TOKEN) {
            error_msg = "Invalid token";
            return false;
        }

        return true;
    }

} // namespace utils::guard
