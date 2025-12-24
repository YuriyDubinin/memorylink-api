#pragma once

#include <chrono>
#include <string>

#include <httplib/httplib.h>
#include <rapidjson/document.h>

#include <config/config_manager.h>
#include <db/db_registry.h>
#include <models/access_token_data.h>
#include <models/api_response.h>
#include <repositories/pg_video_repository.h>
#include <utils/access_token_crypto.h>
#include <utils/format_pg_timestamp.h>
#include <utils/http_response.h>

class VideoService {
public:
    VideoService(httplib::Response& res, rapidjson::Document& body_json, ApiResponse& api_response);

    ~VideoService() = default;

    void GetById();
    void GetListByFamilyId();

private:
    httplib::Response&   res_;
    rapidjson::Document& body_json_;
    ApiResponse&         api_response_;
};