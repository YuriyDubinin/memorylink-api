#include "photo_get.h"

namespace validate {
    bool photo_get(const rapidjson::Document& body_json, ApiResponse& api_response) {
        if (!body_json.HasMember("id")) {
            api_response.status = "FAIL";
            api_response.code   = 400;
            api_response.msg    = "Missing required field 'id'";
            return false;
        }

        if (!body_json["id"].IsNumber()) {
            api_response.status = "FAIL";
            api_response.code   = 422;
            api_response.msg    = "'id' must be a number";
            return false;
        }

        const std::string encrypted_token = body_json["access_token"].GetString();

        try {
            const Config&   cfg = ConfigManager::Get();
            AccessTokenData token_data =
                utils::security::decrypt_access_token_struct(encrypted_token, cfg.pepper, cfg.salt);

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
