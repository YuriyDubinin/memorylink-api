#include "get_photo_list_by_family_id.h"

namespace validate {
    bool get_photo_list_by_family_id(const httplib::Request&    req,
                                     const rapidjson::Document& body_json,
                                     ApiResponse&               api_response) {
        // family_id
        if (!body_json.HasMember("family_id")) {
            api_response.status = "ERROR";
            api_response.code   = 400;
            api_response.msg    = "Missing required field 'family_id'";
            return false;
        }

        if (!body_json["family_id"].IsNumber()) {
            api_response.status = "ERROR";
            api_response.code   = 422;
            api_response.msg    = "'family_id' must be a number";
            return false;
        }

        // offset
        if (!body_json.HasMember("offset")) {
            api_response.status = "ERROR";
            api_response.code   = 400;
            api_response.msg    = "Missing required field 'offset'";
            return false;
        }

        if (!body_json["offset"].IsNumber()) {
            api_response.status = "ERROR";
            api_response.code   = 422;
            api_response.msg    = "'offset' must be a number";
            return false;
        }

        // limit
        if (!body_json.HasMember("limit")) {
            api_response.status = "ERROR";
            api_response.code   = 400;
            api_response.msg    = "Missing required field 'limit'";
            return false;
        }

        if (!body_json["limit"].IsNumber()) {
            api_response.status = "ERROR";
            api_response.code   = 422;
            api_response.msg    = "'limit' must be a number";
            return false;
        }

        std::size_t limit = body_json["limit"].GetUint64();
        if (limit < 20 || limit > 100) {
            api_response.status = "ERROR";
            api_response.code   = 422;
            api_response.msg    = "Field 'limit' must be between 20 and 100";
            return false;
        }

        const std::int64_t family_id = body_json["family_id"].GetInt64();

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
