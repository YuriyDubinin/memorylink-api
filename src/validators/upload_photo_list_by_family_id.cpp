#include "get_photo_list_by_family_id.h"

namespace validate {
    bool upload_photo_list_by_family_id(const httplib::Request& req, ApiResponse& api_response) {
        if (req.form.fields.empty()) {
            api_response.status = "ERROR";
            api_response.code   = 400;
            api_response.msg    = "Fields not found";
            return false;
        }

        // family_id
        auto family_it = req.form.fields.find("family_id");
        if (family_it == req.form.fields.end()) {
            api_response.status = "ERROR";
            api_response.code   = 400;
            api_response.msg    = "Missing required field 'family_id'";
            return false;
        }

        std::string family_id_str = family_it->second.content;

        try {
            size_t       pos       = 0;
            std::int64_t family_id = std::stoll(family_id_str, &pos);

            // Проверка на число
            if (pos != family_id_str.size()) {
                throw std::invalid_argument("extra characters");
            }

            if (family_id <= 0) {
                throw std::out_of_range("non-positive value");
            }
        } catch (const std::exception&) {
            api_response.status = "ERROR";
            api_response.code   = 400;
            api_response.msg    = "'family_id' must be a positive number";
            return false;
        }

        return true;
    }
} // namespace validate
