#include "user_service.h"

UserService::UserService(httplib::Response&   res,
                         rapidjson::Document& body_json,
                         ApiResponse&         api_response)
    : res_(res), body_json_(body_json), api_response_(api_response) {}

void UserService::Auth() {
    const std::string login    = body_json_["login"].GetString();
    const std::string password = body_json_["password"].GetString();

    rapidjson::Document data;
    data.SetObject();
    auto& allocator = data.GetAllocator();

    const bool is_valid = login == "test" && password == "1234";
    if (is_valid) {
        const std::string token = GenerateToken_();

        api_response_.status = "OK";
        api_response_.code   = 200;
        api_response_.msg    = "Authorized";

        data.AddMember("token", rapidjson::Value(token.c_str(), allocator), allocator);
    } else {
        api_response_.status = "ERROR";
        api_response_.code   = 401;
        api_response_.msg    = "Invalid login or password";
    }

    utils::http_response::send(res_, api_response_, data);
}

std::string UserService::GenerateToken_() const {
    return "SECRET_TOKEN";
}
