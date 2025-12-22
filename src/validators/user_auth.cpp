#include "user_auth.h"

namespace validate {
    bool user_auth(const rapidjson::Document& body_json, ApiResponse& api_response) {
        if (!body_json.HasMember("login")) {
            api_response.status = "FAIL";
            api_response.code   = 400;
            api_response.msg    = "Missing required field 'login'";
            return false;
        }

        if (!body_json["login"].IsString()) {
            api_response.status = "FAIL";
            api_response.code   = 422;
            api_response.msg    = "'login' must be a string";
            return false;
        }

        if (!body_json.HasMember("password")) {
            api_response.status = "FAIL";
            api_response.code   = 400;
            api_response.msg    = "Missing required field 'password'";
            return false;
        }

        if (!body_json["password"].IsString()) {
            api_response.status = "FAIL";
            api_response.code   = 422;
            api_response.msg    = "'password' must be a string";
            return false;
        }

        return true;
    }

} // namespace validate
