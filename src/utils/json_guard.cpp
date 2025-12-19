#include "json_guard.hpp"

namespace utils::guard {
    bool parse_json(const httplib::Request& req, httplib::Response& res,
                    rapidjson::Document& out_doc) {
        if (out_doc.Parse(req.body.c_str()).HasParseError()) {
            utils::http_response::send_json_response(res, "ERROR", 400, "Invalid JSON");
            return false;
        }

        if (!out_doc.IsObject()) {
            utils::http_response::send_json_response(res, "ERROR", 400, "JSON must be an object");
            return false;
        }

        return true;
    }
} // namespace utils::guard
