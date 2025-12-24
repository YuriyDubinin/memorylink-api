#include "video_service.h"

VideoService::VideoService(httplib::Response&   res,
                           rapidjson::Document& body_json,
                           ApiResponse&         api_response)
    : res_(res), body_json_(body_json), api_response_(api_response) {}

void VideoService::GetById() {
    const std::int64_t id = body_json_["id"].GetInt64();

    rapidjson::Document data;
    data.SetObject();
    auto& allocator = data.GetAllocator();

    auto video = DBRegistry::VideoRepository().GetById(id);
    if (!video) {
        api_response_.status = "ERROR";
        api_response_.code   = 404;
        api_response_.msg    = "Video not found";

        utils::http_response::send(res_, api_response_, data);

        return;
    }

    const std::string encrypted_token = body_json_["access_token"].GetString();
    const Config&     cfg             = ConfigManager::Get();
    AccessTokenData   token_data =
        utils::security::decrypt_access_token_struct(encrypted_token, cfg.pepper, cfg.salt);

    if (video->family_id != token_data.family_id) {
        api_response_.status = "ERROR";
        api_response_.code   = 403;
        api_response_.msg    = "Access denied: user is attempting an unauthorized action";

        utils::http_response::send(res_, api_response_, data);

        return;
    }

    api_response_.status = "OK";
    api_response_.code   = 200;
    api_response_.msg    = "Success";

    data.AddMember("id", video->id, allocator);
    data.AddMember("family_id", video->family_id, allocator);
    data.AddMember("is_active", video->is_active, allocator);
    data.AddMember("file_size_mb", video->file_size_mb, allocator);

    data.AddMember("name", rapidjson::Value(video->name.c_str(), allocator), allocator);

    data.AddMember("hash", rapidjson::Value(video->hash.c_str(), allocator), allocator);

    data.AddMember("mime_type", rapidjson::Value(video->mime_type.c_str(), allocator), allocator);

    if (video->description) {
        data.AddMember(
            "description", rapidjson::Value(video->description->c_str(), allocator), allocator);
    } else {
        data.AddMember("description", rapidjson::Value(rapidjson::kNullType), allocator);
    }

    if (video->resolution_width_px) {
        data.AddMember("resolution_width_px", *video->resolution_width_px, allocator);
    } else {
        data.AddMember("resolution_width_px", rapidjson::Value(rapidjson::kNullType), allocator);
    }

    if (video->resolution_height_px) {
        data.AddMember("resolution_height_px", *video->resolution_height_px, allocator);
    } else {
        data.AddMember("resolution_height_px", rapidjson::Value(rapidjson::kNullType), allocator);
    }

    if (video->duration_sec) {
        data.AddMember("duration_sec", *video->duration_sec, allocator);
    } else {
        data.AddMember("duration_sec", rapidjson::Value(rapidjson::kNullType), allocator);
    }

    if (video->frame_rate) {
        data.AddMember("frame_rate", *video->frame_rate, allocator);
    } else {
        data.AddMember("frame_rate", rapidjson::Value(rapidjson::kNullType), allocator);
    }

    data.AddMember("created_at", rapidjson::Value(video->created_at.c_str(), allocator), allocator);

    data.AddMember("updated_at", rapidjson::Value(video->updated_at.c_str(), allocator), allocator);

    utils::http_response::send(res_, api_response_, data);
}

void VideoService::GetListByFamilyId() {
    const std::int64_t family_id = body_json_["family_id"].GetInt64();
    const std::size_t  limit     = body_json_["limit"].GetUint64();
    const std::size_t  offset    = body_json_["offset"].GetUint64();

    rapidjson::Document data;
    data.SetObject();
    auto& allocator = data.GetAllocator();

    std::vector<Video> video_list =
        DBRegistry::VideoRepository().GetListByFamilyId(family_id, limit, offset);

    rapidjson::Value videos_json(rapidjson::kArrayType);

    for (const auto& video : video_list) {
        rapidjson::Value item(rapidjson::kObjectType);

        item.AddMember("id", video.id, allocator);
        item.AddMember("family_id", video.family_id, allocator);
        item.AddMember("is_active", video.is_active, allocator);
        item.AddMember("file_size_mb", video.file_size_mb, allocator);
        item.AddMember("name", rapidjson::Value(video.name.c_str(), allocator), allocator);
        item.AddMember("hash", rapidjson::Value(video.hash.c_str(), allocator), allocator);
        item.AddMember(
            "mime_type", rapidjson::Value(video.mime_type.c_str(), allocator), allocator);

        if (video.description) {
            item.AddMember(
                "description", rapidjson::Value(video.description->c_str(), allocator), allocator);
        } else {
            item.AddMember("description", rapidjson::Value(rapidjson::kNullType), allocator);
        }

        if (video.resolution_width_px) {
            item.AddMember("resolution_width_px", *video.resolution_width_px, allocator);
        } else {
            item.AddMember(
                "resolution_width_px", rapidjson::Value(rapidjson::kNullType), allocator);
        }

        if (video.resolution_height_px) {
            item.AddMember("resolution_height_px", *video.resolution_height_px, allocator);
        } else {
            item.AddMember(
                "resolution_height_px", rapidjson::Value(rapidjson::kNullType), allocator);
        }

        item.AddMember(
            "created_at", rapidjson::Value(video.created_at.c_str(), allocator), allocator);
        item.AddMember(
            "updated_at", rapidjson::Value(video.updated_at.c_str(), allocator), allocator);

        videos_json.PushBack(item, allocator);
    }

    api_response_.status = "OK";
    api_response_.code   = 200;
    api_response_.msg    = "Success";

    data.AddMember("list", videos_json, allocator);

    utils::http_response::send(res_, api_response_, data);
}
