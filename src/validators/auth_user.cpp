#include "auth_user.h"

namespace validate {
    bool auth_user(const rapidjson::Document& body_json, ApiResponse& api_response) {
        if (!body_json.HasMember("email")) {
            api_response.status = "ERROR";
            api_response.code   = 400;
            api_response.msg    = "Missing required field 'email'";
            return false;
        }

        if (!body_json["email"].IsString()) {
            api_response.status = "ERROR";
            api_response.code   = 422;
            api_response.msg    = "'email' must be a string";
            return false;
        }

        if (!body_json.HasMember("password")) {
            api_response.status = "ERROR";
            api_response.code   = 400;
            api_response.msg    = "Missing required field 'password'";
            return false;
        }

        if (!body_json["password"].IsString()) {
            api_response.status = "ERROR";
            api_response.code   = 422;
            api_response.msg    = "'password' must be a string";
            return false;
        }

        return true;
    }

} // namespace validate
