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

        const std::string login    = body_json["login"].GetString();
        const std::string password = body_json["password"].GetString();
        const std::string test_hash = "2830fffac1e704d3c6256e13202583185401ba35909018e35039ae4fd9faf246";

        if (!utils::security::verify_password(login, password, test_hash)) {
            api_response.status = "FAIL";
            api_response.code   = 401;
            api_response.msg    = "Invalid 'login' or 'password'";

            return false;
        }

        return true;
    }

} // namespace validate
