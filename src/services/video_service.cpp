#include "video_service.h"

static std::optional<double> parse_double(const std::string& v) {
    if (v.empty())
        return std::nullopt;
    try {
        return std::stod(v);
    } catch (...) {
        return std::nullopt;
    }
}

VideoService::VideoService(const httplib::Request& req,
                           httplib::Response&      res,
                           rapidjson::Document&    body_json,
                           ApiResponse&            api_response)
    : req_(req), res_(res), body_json_(body_json), api_response_(api_response) {}

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

    const auto      encrypted_token_opt = utils::extract_bearer_token(req_);
    const Config&   cfg                 = ConfigManager::Get();
    AccessTokenData token_data =
        utils::security::decrypt_access_token_struct(*encrypted_token_opt, cfg.pepper, cfg.salt);

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

    VideoListResult video_list_res =
        DBRegistry::VideoRepository().GetListByFamilyId(family_id, limit, offset);

    rapidjson::Value videos_json(rapidjson::kArrayType);

    if (video_list_res.count > 0) {
        for (const auto& video : video_list_res.list) {
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
                item.AddMember("description",
                               rapidjson::Value(video.description->c_str(), allocator),
                               allocator);
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
    }

    api_response_.status = "OK";
    api_response_.code   = 200;
    api_response_.msg    = "Success";

    data.AddMember("count", static_cast<int>(video_list_res.count), allocator);
    data.AddMember("list", videos_json, allocator);

    utils::http_response::send(res_, api_response_, data);
}

void VideoService::UploadByFamilyId() {
    std::int64_t family_id = 0;
    auto         it        = req_.form.fields.find("family_id");
    if (it != req_.form.fields.end())
        family_id = std::stoll(it->second.content);

    rapidjson::Document data_json;
    data_json.SetObject();
    auto& allocator = data_json.GetAllocator();

    std::vector<Video> video_vector;
    auto               s3_client = GetS3Client();

    if (!s3_client) {
        api_response_.status = "ERROR";
        api_response_.code   = 500;
        api_response_.msg    = "S3 client not initialized";
        utils::http_response::send(res_, api_response_, data_json);
        return;
    }

    const auto& file = req_.form.files.begin()->second;

    AVFormatContext* fmt_ctx = avformat_alloc_context();
    if (!fmt_ctx)
        return;

    unsigned char* buffer = static_cast<unsigned char*>(av_malloc(file.content.size()));
    if (!buffer) {
        avformat_free_context(fmt_ctx);
        return;
    }
    memcpy(buffer, file.content.data(), file.content.size());

    auto read_data = std::make_pair(reinterpret_cast<const char*>(buffer), file.content.size());

    AVIOContext* io_ctx = avio_alloc_context(
        buffer,
        file.content.size(),
        0,
        &read_data,
        [](void* opaque, uint8_t* buf, int buf_size) -> int {
            auto* data    = reinterpret_cast<std::pair<const char*, size_t>*>(opaque);
            int   to_copy = std::min(buf_size, static_cast<int>(data->second));
            if (to_copy <= 0)
                return AVERROR_EOF;
            memcpy(buf, data->first, to_copy);
            data->first += to_copy;
            data->second -= to_copy;
            return to_copy;
        },
        nullptr,
        nullptr);

    fmt_ctx->pb = io_ctx;

    // Открытие видео из памяти
    if (avformat_open_input(&fmt_ctx, nullptr, nullptr, nullptr) < 0) {
        avformat_free_context(fmt_ctx);
        avio_context_free(&io_ctx);
        api_response_.status = "ERROR";
        api_response_.code   = 400;
        api_response_.msg    = "Uploaded file is not a valid video";
        utils::http_response::send(res_, api_response_, data_json);
        return;
    }

    if (avformat_find_stream_info(fmt_ctx, nullptr) < 0) {
        avformat_close_input(&fmt_ctx);
        avio_context_free(&io_ctx);
        api_response_.status = "ERROR";
        api_response_.code   = 400;
        api_response_.msg    = "Cannot read video stream info";
        utils::http_response::send(res_, api_response_, data_json);
        return;
    }

    // Поиск видеопотока
    AVStream* video_stream = nullptr;
    for (unsigned i = 0; i < fmt_ctx->nb_streams; i++) {
        if (fmt_ctx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            video_stream = fmt_ctx->streams[i];
            break;
        }
    }

    if (!video_stream) {
        avformat_close_input(&fmt_ctx);
        avio_context_free(&io_ctx);

        api_response_.status = "ERROR";
        api_response_.code   = 400;
        api_response_.msg    = "No video stream found";

        utils::http_response::send(res_, api_response_, data_json);

        return; // видеопоток отсутствует, не видео
    }

    Video video;
    video.family_id            = family_id;
    video.name                 = file.filename;
    video.file_size_mb         = static_cast<double>(file.content.size()) / (1024.0 * 1024.0);
    video.mime_type            = file.content_type;
    video.is_active            = true;
    video.resolution_width_px  = video_stream->codecpar->width;
    video.resolution_height_px = video_stream->codecpar->height;
    video.duration_sec         = fmt_ctx->duration / static_cast<double>(AV_TIME_BASE);
    AVRational fr              = video_stream->r_frame_rate;
    video.frame_rate           = fr.den != 0 ? fr.num / static_cast<double>(fr.den) : 0.0;
    video.hash                 = utils::UIDGenerator::generate();

    // Загрузка в S3
    std::string       s3_key = "videos/" + video.hash;
    std::vector<char> data(file.content.begin(), file.content.end());
    if (!s3_client->UploadFromMemory("memorylink-bucket", s3_key, data, video.mime_type)) {
        std::cerr << "[VideoService::UploadByFamilyId]: Warning checksum mismatch for: "
                  << file.filename << std::endl;
    }

    video_vector.emplace_back(std::move(video));

    // Освобождение ресурсов FFmpeg
    avformat_close_input(&fmt_ctx);
    avio_context_free(&io_ctx);

    // Сохранение в БД
    try {
        DBRegistry::VideoRepository().InsertListByFamilyId(family_id, video_vector);
    } catch (const std::exception& e) {
        api_response_.status = "ERROR";
        api_response_.code   = 500;
        api_response_.msg    = e.what();

        utils::http_response::send(res_, api_response_, data_json);

        return;
    }

    data_json.AddMember("count", static_cast<uint64_t>(video_vector.size()), allocator);

    api_response_.status = "OK";
    api_response_.code   = 200;
    api_response_.msg    = "Video uploaded successfully";

    utils::http_response::send(res_, api_response_, data_json);
}
