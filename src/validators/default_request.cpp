#include "default_request.h"

namespace validate {
    bool default_request(const httplib::Request& req,
                         httplib::Response&      res,
                         rapidjson::Document&    body_json,
                         ApiResponse&            api_response) {
        if (!validate::json(req, body_json, api_response) ||
            !validate::access_token(req, api_response)) {
            utils::http_response::send(res, api_response);
            return false;
        }
        return true;
    }

} // namespace validate