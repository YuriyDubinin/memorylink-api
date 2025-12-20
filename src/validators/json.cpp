#include "json.hpp"

namespace validate {
    bool json(const httplib::Request& req, httplib::Response& res, rapidjson::Document& out_doc,
              std::string& error_msg) {
        if (out_doc.Parse(req.body.c_str()).HasParseError()) {
            error_msg = "Invalid JSON";
            return false;
        }

        if (!out_doc.IsObject()) {
            error_msg = "JSON must be an object";
            return false;
        }

        return true;
    }
} // namespace validate
