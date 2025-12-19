#include "check_token.hpp"

namespace utils::guard {
    constexpr std::string EXPECTED_TOKEN = "SECRET_TOKEN";

    bool check_token(const rapidjson::Document& json, httplib::Response& res) {
        if (!json.HasMember("token")) {
            utils::http_response::send_json_response(res, "ERROR", 400, "Missing 'token'");
            return false;
        }

        if (!json["token"].IsString()) {
            utils::http_response::send_json_response(res, "ERROR", 400, "'token' must be a string");
            return false;
        }

        const std::string token = json["token"].GetString();
        if (token != EXPECTED_TOKEN) {
            utils::http_response::send_json_response(res, "ERROR", 403, "Invalid token");
            return false;
        }

        return true;
    }

} // namespace utils::guard
