#include "photo_service.h"

PhotoService::PhotoService(httplib::Response&   res,
                           rapidjson::Document& body_json,
                           ApiResponse&         api_response)
    : res_(res), body_json_(body_json), api_response_(api_response) {}

void PhotoService::GetById() {
    const std::int64_t id = body_json_["id"].GetInt64();

    rapidjson::Document data;
    data.SetObject();
    auto& allocator = data.GetAllocator();

    auto photo = DBRegistry::PhotoRepository().GetById(id);
    if (!photo) {
        api_response_.status = "ERROR";
        api_response_.code   = 404;
        api_response_.msg    = "Photo not found";

        utils::http_response::send(res_, api_response_, data);

        return;
    }

    const std::string encrypted_token = body_json_["access_token"].GetString();
    const Config&     cfg             = ConfigManager::Get();
    AccessTokenData   token_data =
        utils::security::decrypt_access_token_struct(encrypted_token, cfg.pepper, cfg.salt);

    if (photo->family_id != token_data.family_id) {
        api_response_.status = "ERROR";
        api_response_.code   = 403;
        api_response_.msg    = "Access denied: user is attempting an unauthorized action";

        utils::http_response::send(res_, api_response_, data);

        return;
    }

    api_response_.status = "OK";
    api_response_.code   = 200;
    api_response_.msg    = "Success";

    data.AddMember("id", photo->id, allocator);
    data.AddMember("family_id", photo->family_id, allocator);
    data.AddMember("is_active", photo->is_active, allocator);
    data.AddMember("file_size_mb", photo->file_size_mb, allocator);

    data.AddMember("name", rapidjson::Value(photo->name.c_str(), allocator), allocator);

    data.AddMember("hash", rapidjson::Value(photo->hash.c_str(), allocator), allocator);

    data.AddMember("mime_type", rapidjson::Value(photo->mime_type.c_str(), allocator), allocator);

    if (photo->description) {
        data.AddMember(
            "description", rapidjson::Value(photo->description->c_str(), allocator), allocator);
    } else {
        data.AddMember("description", rapidjson::Value(rapidjson::kNullType), allocator);
    }

    if (photo->resolution_width_px) {
        data.AddMember("resolution_width_px", *photo->resolution_width_px, allocator);
    } else {
        data.AddMember("resolution_width_px", rapidjson::Value(rapidjson::kNullType), allocator);
    }

    if (photo->resolution_height_px) {
        data.AddMember("resolution_height_px", *photo->resolution_height_px, allocator);
    } else {
        data.AddMember("resolution_height_px", rapidjson::Value(rapidjson::kNullType), allocator);
    }

    data.AddMember("created_at", rapidjson::Value(photo->created_at.c_str(), allocator), allocator);

    data.AddMember("updated_at", rapidjson::Value(photo->updated_at.c_str(), allocator), allocator);

    utils::http_response::send(res_, api_response_, data);
}

void PhotoService::GetListByFamilyId() {
    const std::int64_t family_id = body_json_["family_id"].GetInt64();
    const std::size_t  limit     = body_json_["limit"].GetUint64();
    const std::size_t  offset    = body_json_["offset"].GetUint64();

    rapidjson::Document data;
    data.SetObject();
    auto& allocator = data.GetAllocator();

    PhotoListResult photo_list_res =
        DBRegistry::PhotoRepository().GetListByFamilyId(family_id, limit, offset);

    rapidjson::Value photos_json(rapidjson::kArrayType);

    if (photo_list_res.count > 0) {
        for (const auto& photo : photo_list_res.list) {
            rapidjson::Value item(rapidjson::kObjectType);

            item.AddMember("id", photo.id, allocator);
            item.AddMember("family_id", photo.family_id, allocator);
            item.AddMember("is_active", photo.is_active, allocator);
            item.AddMember("file_size_mb", photo.file_size_mb, allocator);
            item.AddMember("name", rapidjson::Value(photo.name.c_str(), allocator), allocator);
            item.AddMember("hash", rapidjson::Value(photo.hash.c_str(), allocator), allocator);
            item.AddMember(
                "mime_type", rapidjson::Value(photo.mime_type.c_str(), allocator), allocator);

            if (photo.description) {
                item.AddMember("description",
                               rapidjson::Value(photo.description->c_str(), allocator),
                               allocator);
            } else {
                item.AddMember("description", rapidjson::Value(rapidjson::kNullType), allocator);
            }

            if (photo.resolution_width_px) {
                item.AddMember("resolution_width_px", *photo.resolution_width_px, allocator);
            } else {
                item.AddMember(
                    "resolution_width_px", rapidjson::Value(rapidjson::kNullType), allocator);
            }

            if (photo.resolution_height_px) {
                item.AddMember("resolution_height_px", *photo.resolution_height_px, allocator);
            } else {
                item.AddMember(
                    "resolution_height_px", rapidjson::Value(rapidjson::kNullType), allocator);
            }

            item.AddMember(
                "created_at", rapidjson::Value(photo.created_at.c_str(), allocator), allocator);
            item.AddMember(
                "updated_at", rapidjson::Value(photo.updated_at.c_str(), allocator), allocator);

            photos_json.PushBack(item, allocator);
        }
    }

    api_response_.status = "OK";
    api_response_.code   = 200;
    api_response_.msg    = "Success";

    data.AddMember("count", static_cast<int>(photo_list_res.count), allocator);
    data.AddMember("list", photos_json, allocator);

    utils::http_response::send(res_, api_response_, data);
}
