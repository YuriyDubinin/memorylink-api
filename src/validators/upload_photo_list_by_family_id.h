#pragma once

#include <string>

#include <httplib/httplib.h>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include <config/config_manager.h>
#include <models/access_token_data.h>
#include <models/api_response.h>
#include <utils/access_token_crypto.h>
#include <utils/http_response.h>

namespace validate {
    bool upload_photo_list_by_family_id(const httplib::Request&    req,
                                        const rapidjson::Document& body_json,
                                        ApiResponse&               api_response);
}