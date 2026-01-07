#include "photo_service.h"

PhotoService::PhotoService(const httplib::Request& req,
                           httplib::Response&      res,
                           rapidjson::Document&    body_json,
                           ApiResponse&            api_response)
    : req_(req), res_(res), body_json_(body_json), api_response_(api_response) {}

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

    const auto      encrypted_token_opt = utils::extract_bearer_token(req_);
    const Config&   cfg                 = ConfigManager::Get();
    AccessTokenData token_data =
        utils::security::decrypt_access_token_struct(*encrypted_token_opt, cfg.pepper, cfg.salt);

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

void PhotoService::UploadListByFamilyId() {
    std::int64_t family_id = 0;
    auto         it        = req_.form.fields.find("family_id");
    if (it != req_.form.fields.end()) {
        family_id = std::stoll(it->second.content);
    }

    rapidjson::Document data_json;
    data_json.SetObject();
    auto& allocator = data_json.GetAllocator();

    std::vector<Photo> photo_vector;
    auto               s3_client = GetS3Client();

    if (!s3_client) {
        api_response_.status = "ERROR";
        api_response_.code   = 500;
        api_response_.msg    = "S3 client not initialized";
        utils::http_response::send(res_, api_response_, data_json);
        return;
    }

    for (const auto& [field_name, file] : req_.form.files) {
        int width = 0, height = 0;

        if (!stbi_info_from_memory(reinterpret_cast<const unsigned char*>(file.content.data()),
                                   static_cast<int>(file.content.size()),
                                   &width,
                                   &height,
                                   nullptr)) {
            continue; // не изображение
        }

        Photo photo;
        photo.family_id            = family_id;
        photo.name                 = file.filename;
        photo.mime_type            = file.content_type;
        photo.file_size_mb         = static_cast<double>(file.content.size()) / (1024.0 * 1024.0);
        photo.is_active            = true;
        photo.hash                 = utils::UIDGenerator::generate();
        photo.resolution_width_px  = width;
        photo.resolution_height_px = height;

        // Загрузка в S3
        std::string       s3_key = "photos/" + photo.hash;
        std::vector<char> data(file.content.begin(), file.content.end());
        if (!s3_client->UploadFromMemory("memorylink-bucket", s3_key, data, photo.mime_type)) {
            std::cerr << "[PhotoService]: Failed to upload to S3: " << file.filename << std::endl;
            continue;
        }

        photo_vector.emplace_back(std::move(photo));
    }

    // Сохранение в БД
    try {
        DBRegistry::PhotoRepository().InsertListByFamilyId(family_id, photo_vector);
    } catch (const std::exception& e) {
        api_response_.status = "ERROR";
        api_response_.code   = 500;
        api_response_.msg    = e.what();

        utils::http_response::send(res_, api_response_, data_json);
        return;
    }

    data_json.AddMember("count", static_cast<uint64_t>(photo_vector.size()), allocator);

    api_response_.status = "OK";
    api_response_.code   = 200;
    api_response_.msg    = "Photos uploaded successfully";

    utils::http_response::send(res_, api_response_, data_json);
}
