#include "video_service.h"

VideoService::VideoService(httplib::Response&    res,
                           rapidjson::Document& body_json,
                           ApiResponse&         api_response)
    : res_(res), body_json_(body_json), api_response_(api_response) {}

void VideoService::GetById() {
    const std::int64_t id = body_json_["id"].GetInt64();

    rapidjson::Document data;
    data.SetObject();
    auto& allocator = data.GetAllocator();

    auto video = DBRegistry::VideoRepository().GetById(id);

    if (video) {
        api_response_.status = "OK";
        api_response_.code   = 200;
        api_response_.msg    = "Success";

        data.AddMember("id", video->id, allocator);
        data.AddMember("family_id", video->family_id, allocator);
        data.AddMember("is_active", video->is_active, allocator);
        data.AddMember("file_size_mb", video->file_size_mb, allocator);

        data.AddMember(
            "name", rapidjson::Value(video->name.c_str(), allocator), allocator);

        data.AddMember(
            "hash", rapidjson::Value(video->hash.c_str(), allocator), allocator);

        data.AddMember(
            "mime_type", rapidjson::Value(video->mime_type.c_str(), allocator), allocator);

        if (video->description) {
            data.AddMember(
                "description",
                rapidjson::Value(video->description->c_str(), allocator),
                allocator);
        } else {
            data.AddMember("description", rapidjson::Value(rapidjson::kNullType), allocator);
        }

        if (video->resolution_width_px) {
            data.AddMember("resolution_width_px", *video->resolution_width_px, allocator);
        } else {
            data.AddMember(
                "resolution_width_px", rapidjson::Value(rapidjson::kNullType), allocator);
        }

        if (video->resolution_height_px) {
            data.AddMember("resolution_height_px", *video->resolution_height_px, allocator);
        } else {
            data.AddMember(
                "resolution_height_px", rapidjson::Value(rapidjson::kNullType), allocator);
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

        data.AddMember(
            "created_at",
            rapidjson::Value(video->created_at.c_str(), allocator),
            allocator);

        data.AddMember(
            "updated_at",
            rapidjson::Value(video->updated_at.c_str(), allocator),
            allocator);

    } else {
        api_response_.status = "ERROR";
        api_response_.code   = 404;
        api_response_.msg    = "Video not found";
    }

    utils::http_response::send(res_, api_response_, data);
}
