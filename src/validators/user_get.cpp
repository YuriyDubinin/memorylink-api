#include "user_get.h"

namespace validate {
    bool user_get(const rapidjson::Document& body, ApiResponse& api_response) {
        if (!body.HasMember("id")) {
            api_response.status = "FAIL";
            api_response.code   = 400;
            api_response.msg    = "Missing required field 'id'";
            return false;
        }

        if (!body["id"].IsNumber()) {
            api_response.status = "FAIL";
            api_response.code   = 422;
            api_response.msg    = "'id' must be a number";
            return false;
        }

        return true;
    }

} // namespace validate
