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

    auto user_entity = DBRegistry::Users().GetByEmail(login);

    if (user_entity &&
        utils::security::verify_password(login, password, user_entity->password_hash)) {

        AccessTokenData access_token_data;
        access_token_data.id = user_entity->id;
        access_token_data.family_id = user_entity->family_id;

        const std::string token = GenerateToken_(access_token_data);

        api_response_.status = "OK";
        api_response_.code   = 200;
        api_response_.msg    = "Authorized";

        data.AddMember("token", rapidjson::Value(token.c_str(), allocator), allocator);
    } else {
        api_response_.status = "ERROR";
        api_response_.code   = 401;
        api_response_.msg    = "Invalid 'login' or 'password'";
    }

    utils::http_response::send(res_, api_response_, data);
}

void UserService::Get() {}

std::string UserService::GenerateToken_(AccessTokenData& access_token_data) const {
    // TTL = текущее время + 24 часа (в секундах)
    access_token_data.ttl =
        std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::system_clock::now().time_since_epoch()
        ).count()
        + 24 * 60 * 60;

    const Config&      cfg = ConfigManager::Get();
    static const std::string TOKEN_KEY  = cfg.pepper;
    static const std::string TOKEN_SALT = cfg.salt;

    return utils::security::encrypt_access_token_struct(
        access_token_data,
        TOKEN_KEY,
        TOKEN_SALT
    );
}
