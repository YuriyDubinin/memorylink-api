#include "get_family_by_id.h"

namespace validate {
    bool get_family_by_id(const httplib::Request&    req,
                          const rapidjson::Document& body_json,
                          ApiResponse&               api_response) {
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

        const auto encrypted_token_opt = utils::extract_bearer_token(req);

        try {
            const Config&   cfg        = ConfigManager::Get();
            AccessTokenData token_data = utils::security::decrypt_access_token_struct(
                *encrypted_token_opt, cfg.pepper, cfg.salt);

            if (body_json["id"].GetInt() != token_data.family_id) {
                api_response.status = "ERROR";
                api_response.code   = 403;
                api_response.msg    = "Access denied: user is attempting an unauthorized action";
                return false;
            }

            return true;

        } catch (const std::exception& e) {
            api_response.status = "ERROR";
            api_response.code   = 401;
            api_response.msg    = "Invalid access token";
            return false;
        }
    }
} // namespace validate
