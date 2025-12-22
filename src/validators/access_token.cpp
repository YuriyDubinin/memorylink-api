#include "access_token.h"

namespace validate {
    bool access_token(const rapidjson::Document& body_json, ApiResponse& api_response) {
        if (!body_json.HasMember("access_token")) {
            api_response.status = "ERROR";
            api_response.code   = 400;
            api_response.msg    = "Missing 'access_token'";
            return false;
        }

        if (!body_json["access_token"].IsString()) {
            api_response.status = "ERROR";
            api_response.code   = 400;
            api_response.msg    = "'access_token' must be a string";
            return false;
        }

        const std::string encrypted_token = body_json["access_token"].GetString();

        try {
            const Config& cfg = ConfigManager::Get();
            AccessTokenData token_data =
                utils::security::decrypt_access_token_struct(
                    encrypted_token,
                    cfg.pepper,
                    cfg.salt
                );

            // Текущее время (unix timestamp)
            const std::int64_t now =
                std::chrono::system_clock::to_time_t(
                    std::chrono::system_clock::now());

            if (now > token_data.ttl) {
                api_response.status = "ERROR";
                api_response.code   = 401;
                api_response.msg    = "Expired token";
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
