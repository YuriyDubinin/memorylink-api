#include "upload_photo_list_by_family_id.h"

namespace validate {
    bool upload_photo_list_by_family_id(const httplib::Request&    req,
                                        const rapidjson::Document& body_json,
                                        ApiResponse&               api_response) {
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

        const std::string family_id_str = family_it->second.content;
        std::int64_t      family_id     = 0;

        try {
            size_t pos = 0;
            family_id  = std::stoll(family_id_str, &pos);

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

        const auto      encrypted_token_opt = utils::extract_bearer_token(req);
        const Config&   cfg                 = ConfigManager::Get();
        AccessTokenData token_data          = utils::security::decrypt_access_token_struct(
            *encrypted_token_opt, cfg.pepper, cfg.salt);

        if (family_id != token_data.family_id) {
            api_response.status = "ERROR";
            api_response.code   = 403;
            api_response.msg    = "Access denied: user is attempting an unauthorized action";

            return false;
        }

        return true;
    }
} // namespace validate
