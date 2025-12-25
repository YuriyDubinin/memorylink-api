#pragma once

#include <chrono>
#include <string>

#include <httplib/httplib.h>
#include <openssl/sha.h>

#include <config/config_manager.h>
#include <db/db_registry.h>
#include <models/access_token_data.h>
#include <models/api_response.h>
#include <models/photo.h>
#include <repositories/pg_photo_repository.h>
#include <utils/access_token_crypto.h>
#include <utils/extract_bearer_token.h>
#include <utils/format_pg_timestamp.h>
#include <utils/http_response.h>
#include <utils/uid_generator.h>
#include <validators/upload_photo_list_by_family_id.h>

#include <rapidjson/document.h>
#include <stb/stb_image.h>

class PhotoService {
public:
    PhotoService(const httplib::Request& req,
                 httplib::Response&      res,
                 rapidjson::Document&    body_json,
                 ApiResponse&            api_response);

    ~PhotoService() = default;

    void GetById();
    void GetListByFamilyId();
    void UploadListByFamilyId();

private:
    const httplib::Request& req_;
    httplib::Response&      res_;
    rapidjson::Document&    body_json_;
    ApiResponse&            api_response_;
};