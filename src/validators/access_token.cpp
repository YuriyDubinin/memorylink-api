#include "access_token.h"

#include <chrono>

namespace validate {
    bool access_token(const httplib::Request& req, ApiResponse& api_response) {
        // Authorization header
        auto auth_it = req.headers.find("Authorization");
        if (auth_it == req.headers.end()) {
            api_response.status = "ERROR";
            api_response.code   = 401;
            api_response.msg    = "Missing Authorization header";
            return false;
        }

        const std::string& auth_header = auth_it->second;
        const std::string  bearer      = "Bearer ";

        // Check format: Bearer <token>
        if (auth_header.rfind(bearer, 0) != 0) {
            api_response.status = "ERROR";
            api_response.code   = 401;
            api_response.msg    = "Invalid Authorization header format";
            return false;
        }

        const std::string encrypted_token = auth_header.substr(bearer.size());

        try {
            const Config& cfg = ConfigManager::Get();

            AccessTokenData token_data =
                utils::security::decrypt_access_token_struct(encrypted_token, cfg.pepper, cfg.salt);

            const std::int64_t now =
                std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

            if (now > token_data.ttl) {
                api_response.status = "ERROR";
                api_response.code   = 401;
                api_response.msg    = "Expired token";
                return false;
            }

            return true;

        } catch (const std::exception&) {
            api_response.status = "ERROR";
            api_response.code   = 401;
            api_response.msg    = "Invalid access token";
            return false;
        }
    }

} // namespace validate
