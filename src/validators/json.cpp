#include "json.h"

namespace validate {
    bool
    json(const httplib::Request& req, rapidjson::Document& body_json, ApiResponse& api_response) {
        if (body_json.Parse(req.body.c_str()).HasParseError()) {
            api_response.status = "ERROR";
            api_response.code   = 400;
            api_response.msg    = "Invalid JSON";
            return false;
        }

        if (!body_json.IsObject()) {
            api_response.status = "ERROR";
            api_response.code   = 400;
            api_response.msg    = "JSON must be an object";
            return false;
        }

        return true;
    }
} // namespace validate
