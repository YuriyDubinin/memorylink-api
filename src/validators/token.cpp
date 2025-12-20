#include "token.h"

namespace validate {
    const std::string EXPECTED_TOKEN = "SECRET_TOKEN";

    bool token(const rapidjson::Document& body_json, ApiResponse& api_response) {
        if (!body_json.HasMember("token")) {
            api_response.status = "ERROR";
            api_response.code   = 400;
            api_response.msg    = "Missing 'token'";
            return false;
        }

        if (!body_json["token"].IsString()) {
            api_response.status = "ERROR";
            api_response.code   = 400;
            api_response.msg    = "'token' must be a string";
            return false;
        }

        const std::string token = body_json["token"].GetString();
        if (token != EXPECTED_TOKEN) {
            api_response.status = "ERROR";
            api_response.code   = 401;
            api_response.msg    = "Invalid token";
            return false;
        }

        return true;
    }

} // namespace validate
