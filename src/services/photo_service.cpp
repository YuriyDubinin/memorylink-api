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

    if (photo) {
        api_response_.status = "OK";
        api_response_.code   = 200;
        api_response_.msg    = "Success";

        data.AddMember("id", photo->id, allocator);
        data.AddMember("family_id", photo->family_id, allocator);
        data.AddMember("is_active", photo->is_active, allocator);
        data.AddMember("file_size_mb", photo->file_size_mb, allocator);

        data.AddMember("name", rapidjson::Value(photo->name.c_str(), allocator), allocator);

        data.AddMember("hash", rapidjson::Value(photo->hash.c_str(), allocator), allocator);

        data.AddMember(
            "mime_type", rapidjson::Value(photo->mime_type.c_str(), allocator), allocator);

        if (photo->description) {
            data.AddMember(
                "description", rapidjson::Value(photo->description->c_str(), allocator), allocator);
        } else {
            data.AddMember("description", rapidjson::Value(rapidjson::kNullType), allocator);
        }

        if (photo->resolution_width_px) {
            data.AddMember("resolution_width_px", *photo->resolution_width_px, allocator);
        } else {
            data.AddMember(
                "resolution_width_px", rapidjson::Value(rapidjson::kNullType), allocator);
        }

        if (photo->resolution_height_px) {
            data.AddMember("resolution_height_px", *photo->resolution_height_px, allocator);
        } else {
            data.AddMember(
                "resolution_height_px", rapidjson::Value(rapidjson::kNullType), allocator);
        }

        data.AddMember(
            "created_at", rapidjson::Value(photo->created_at.c_str(), allocator), allocator);

        data.AddMember(
            "updated_at", rapidjson::Value(photo->updated_at.c_str(), allocator), allocator);

    } else {
        api_response_.status = "ERROR";
        api_response_.code   = 404;
        api_response_.msg    = "Photo not found";
    }

    utils::http_response::send(res_, api_response_, data);
}
