#include "get_video_by_id.h"

namespace validate {
    bool get_video_by_id(const rapidjson::Document& body_json, ApiResponse& api_response) {
        if (!body_json.HasMember("id")) {
            api_response.status = "ERROR";
            api_response.code   = 400;
            api_response.msg    = "Missing required field 'id'";
            return false;
        }

        if (!body_json["id"].IsNumber()) {
            api_response.status = "ERROR";
            api_response.code   = 422;
            api_response.msg    = "'id' must be a number";
            return false;
        }

        return true;
    }
} // namespace validate
