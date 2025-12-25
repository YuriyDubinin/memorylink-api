#include "json_request.h"

namespace validate {
    bool json_request(const httplib::Request& req,
                      httplib::Response&      res,
                      rapidjson::Document&    body_json,
                      ApiResponse&            api_response) {
        if (!validate::access_token(req, api_response) ||
            !validate::json(req, body_json, api_response)) {
            return false;
        }
        return true;
    }
} // namespace validate