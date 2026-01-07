#pragma once

#include <chrono>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <optional>
#include <string>

#include <httplib/httplib.h>

#include <config/config_manager.h>
#include <db/db_registry.h>
#include <models/access_token_data.h>
#include <models/api_response.h>
#include <network/s3_client.h>
#include <repositories/pg_video_repository.h>
#include <utils/access_token_crypto.h>
#include <utils/extract_bearer_token.h>
#include <utils/format_pg_timestamp.h>
#include <utils/http_response.h>
#include <utils/uid_generator.h>

#include <rapidjson/document.h>

// Подключение FFmpeg для C++
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavdevice/avdevice.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
#include <libavutil/mathematics.h>
#include <libswresample/swresample.h>
#include <libswscale/swscale.h>
}

class VideoService {
public:
    VideoService(const httplib::Request& req,
                 httplib::Response&      res,
                 rapidjson::Document&    body_json,
                 ApiResponse&            api_response);

    ~VideoService() = default;

    void GetById();
    void GetListByFamilyId();
    void UploadByFamilyId();

private:
    const httplib::Request& req_;
    httplib::Response&      res_;
    rapidjson::Document&    body_json_;
    ApiResponse&            api_response_;
};