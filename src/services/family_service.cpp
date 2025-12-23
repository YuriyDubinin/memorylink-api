#include "family_service.h"

FamilyService::FamilyService(httplib::Response&   res,
                             rapidjson::Document& body_json,
                             ApiResponse&         api_response)
    : res_(res), body_json_(body_json), api_response_(api_response) {}

void FamilyService::GetById() {
    const int id = body_json_["id"].GetInt();

    rapidjson::Document data;
    data.SetObject();
    auto& allocator = data.GetAllocator();

    auto family_entity = DBRegistry::FamilyRepository().GetById(id);
    if (family_entity) {
        api_response_.status = "OK";
        api_response_.code   = 200;
        api_response_.msg    = "Success";

        data.AddMember("id", family_entity->id, allocator);
        data.AddMember("name", rapidjson::Value(family_entity->name.c_str(), allocator), allocator);

        if (family_entity->description.has_value()) {
            data.AddMember("description",
                           rapidjson::Value(family_entity->description->c_str(), allocator),
                           allocator);
        } else {
            data.AddMember("description", rapidjson::Value(rapidjson::kNullType), allocator);
        }

        data.AddMember("owner_id", family_entity->owner_id, allocator);
        data.AddMember("tariff_id", family_entity->tariff_id, allocator);

        // Статус как строка
        std::string status_str;
        switch (family_entity->status) {
            case FamilyStatus::ACTIVE:
                status_str = "ACTIVE";
                break;
            case FamilyStatus::FROZEN:
                status_str = "FROZEN";
                break;
            case FamilyStatus::DELETED:
                status_str = "DELETED";
                break;
        }
        data.AddMember("status", rapidjson::Value(status_str.c_str(), allocator), allocator);

        if (family_entity->status_description.has_value()) {
            data.AddMember("status_description",
                           rapidjson::Value(family_entity->status_description->c_str(), allocator),
                           allocator);
        } else {
            data.AddMember("status_description", rapidjson::Value(rapidjson::kNullType), allocator);
        }

        data.AddMember("storage_limit_mb", family_entity->storage_limit_mb, allocator);
        data.AddMember("storage_used_mb", family_entity->storage_used_mb, allocator);
    } else {
        api_response_.status = "ERROR";
        api_response_.code   = 401;
        api_response_.msg    = "Invalid 'id";
    }

    utils::http_response::send(res_, api_response_, data);
}
