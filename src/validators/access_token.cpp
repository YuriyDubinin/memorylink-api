#include "access_token.h"

namespace validate {
    bool token(const rapidjson::Document& body_json, ApiResponse& api_response) {
        if (!body_json.HasMember("access_token")) {
            api_response.status = "ERROR";
            api_response.code   = 400;
            api_response.msg    = "Missing 'access_token'";
            return false;
        }

        if (!body_json["access_token"].IsString()) {
            api_response.status = "ERROR";
            api_response.code   = 400;
            api_response.msg    = "'access_token' must be a string";
            return false;
        }

        const std::string access_token = body_json["access_token"].GetString();

        // Проверка TTL
        // std::int64_t now =
        // std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()); if (now >
        // stored_token.ttl) {
        //     api_response.status = "ERROR";
        //     api_response.code   = 401;
        //     api_response.msg    = "Token expired";
        //     return false;
        // }

        // const std::string token = body_json["access_token"].GetString();
        // if (token != EXPECTED_TOKEN) {
        //     api_response.status = "ERROR";
        //     api_response.code   = 401;
        //     api_response.msg    = "Invalid token";
        //     return false;
        // }

        return true;
    }

} // namespace validate
