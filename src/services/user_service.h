#pragma once

#include <chrono>
#include <string>

#include <httplib/httplib.h>

#include <config/config_manager.h>
#include <db/db_registry.h>
#include <models/access_token_data.h>
#include <models/api_response.h>
#include <repositories/pg_family_repository.h>
#include <repositories/pg_user_repository.h>
#include <utils/access_token_crypto.h>
#include <utils/http_response.h>
#include <utils/password.h>

#include <rapidjson/document.h>

class UserService {
public:
    UserService(httplib::Response& res, rapidjson::Document& body_json, ApiResponse& api_response);

    ~UserService() = default;

    void Auth();
    void GetById();

private:
    httplib::Response&   res_;
    rapidjson::Document& body_json_;
    ApiResponse&         api_response_;

    [[nodiscard]] std::string GenerateToken_(AccessTokenData& access_token_data) const;
};