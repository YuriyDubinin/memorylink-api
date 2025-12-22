#pragma once

#include <string>

#include <httplib/httplib.h>
#include <rapidjson/document.h>

#include <models/api_response.h>
#include <utils/http_response.h>

class UserService {
public:
    UserService(httplib::Response& res, rapidjson::Document& body_json, ApiResponse& api_response);

    ~UserService() = default;

    void Auth();
    void Get();

private:
    httplib::Response&   res_;
    rapidjson::Document& body_json_;
    ApiResponse&         api_response_;

    [[nodiscard]] std::string GenerateToken_() const;
};