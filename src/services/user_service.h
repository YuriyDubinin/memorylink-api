#pragma once

#include <string>

#include <httplib/httplib.h>
#include <rapidjson/document.h>

#include <models/api_response.h>

class UserService {
public:
    UserService()  = default;
    ~UserService() = default;

    void Auth(httplib::Response& res, rapidjson::Document& body_json, ApiResponse& api_response);

private:
    [[nodiscard]] const std::string GenerateToken_() const;
};