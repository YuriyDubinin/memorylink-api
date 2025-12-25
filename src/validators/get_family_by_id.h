#pragma once

#include <string>

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include <config/config_manager.h>
#include <models/access_token_data.h>
#include <models/api_response.h>
#include <utils/access_token_crypto.h>
#include <utils/http_response.h>
#include <utils/extract_bearer_token.h>

namespace validate {
    bool get_family_by_id(const httplib::Request&    req,
                          const rapidjson::Document& body_json,
                          ApiResponse&               api_response);
}