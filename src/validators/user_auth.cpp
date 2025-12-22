#include "user_auth.h"

namespace validate {
    bool user_auth(const rapidjson::Document& body, ApiResponse& api_response) {
        if (!body.HasMember("login")) {
            api_response.status = "FAIL";
            api_response.code   = 400;
            api_response.msg    = "Missing required field 'login'";
            return false;
        }

        if (!body["login"].IsString()) {
            api_response.status = "FAIL";
            api_response.code   = 422;
            api_response.msg    = "'login' must be a string";
            return false;
        }

        if (!body.HasMember("password")) {
            api_response.status = "FAIL";
            api_response.code   = 400;
            api_response.msg    = "Missing required field 'password'";
            return false;
        }

        if (!body["password"].IsString()) {
            api_response.status = "FAIL";
            api_response.code   = 422;
            api_response.msg    = "'password' must be a string";
            return false;
        }

        return true;
    }

} // namespace validate
