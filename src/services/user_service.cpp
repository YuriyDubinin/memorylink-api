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

    auto user_entity = DBRegistry::UserRepository().GetByEmail(login);
    if (user_entity &&
        utils::security::verify_password(login, password, user_entity->password_hash)) {

        AccessTokenData access_token_data;
        access_token_data.id        = user_entity->id;
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

void UserService::GetById() {
    const int id = body_json_["id"].GetInt();

    rapidjson::Document data;
    data.SetObject();
    auto& allocator = data.GetAllocator();

    auto user_entity = DBRegistry::UserRepository().GetById(id);
    if (user_entity) {
        api_response_.status = "OK";
        api_response_.code   = 200;
        api_response_.msg    = "Success";

        data.AddMember("id", user_entity->id, allocator);
        data.AddMember(
            "full_name", rapidjson::Value(user_entity->full_name.c_str(), allocator), allocator);
        data.AddMember("email", rapidjson::Value(user_entity->email.c_str(), allocator), allocator);

        if (user_entity->phone.has_value()) {
            data.AddMember(
                "phone", rapidjson::Value(user_entity->phone->c_str(), allocator), allocator);
        } else {
            data.AddMember("phone", rapidjson::Value(rapidjson::kNullType), allocator);
        }

        if (user_entity->address.has_value()) {
            data.AddMember(
                "address", rapidjson::Value(user_entity->address->c_str(), allocator), allocator);
        } else {
            data.AddMember("address", rapidjson::Value(rapidjson::kNullType), allocator);
        }

        data.AddMember("family_id", user_entity->family_id, allocator);

        if (user_entity->avatar)
            data.AddMember(
                "avatar", rapidjson::Value(user_entity->avatar->c_str(), allocator), allocator);
        else
            data.AddMember("avatar", rapidjson::Value(rapidjson::kNullType), allocator);

        data.AddMember(
            "created_at", rapidjson::Value(user_entity->created_at.c_str(), allocator), allocator);
        data.AddMember(
            "updated_at", rapidjson::Value(user_entity->updated_at.c_str(), allocator), allocator);

        std::string user_status_str;
        switch (user_entity->status) {
            case UserStatus::ACTIVE:
                user_status_str = "ACTIVE";
                break;
            case UserStatus::DELETED:
                user_status_str = "DELETED";
                break;
        }
        data.AddMember("status", rapidjson::Value(user_status_str.c_str(), allocator), allocator);

        std::string user_role_str;
        switch (user_entity->role) {
            case UserRole::OWNER:
                user_role_str = "OWNER";
                break;
            case UserRole::ADMIN:
                user_role_str = "ADMIN";
                break;
            case UserRole::READER:
                user_role_str = "READER";
                break;
        }
        data.AddMember("role", rapidjson::Value(user_role_str.c_str(), allocator), allocator);
    } else {
        api_response_.status = "ERROR";
        api_response_.code   = 401;
        api_response_.msg    = "Invalid 'id";
    }

    utils::http_response::send(res_, api_response_, data);
}

std::string UserService::GenerateToken_(AccessTokenData& access_token_data) const {
    // TTL = текущее время + 24 часа (в секундах)
    access_token_data.ttl = std::chrono::duration_cast<std::chrono::seconds>(
                                std::chrono::system_clock::now().time_since_epoch())
                                .count() +
                            24 * 60 * 60;

    const Config& cfg = ConfigManager::Get();

    return utils::security::encrypt_access_token_struct(access_token_data, cfg.pepper, cfg.salt);
}
