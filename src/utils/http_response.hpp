#pragma once

#include <httplib/httplib.h>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <string>

namespace utils::http_response {

    inline void SendJsonResponse(httplib::Response& res,
                                 const std::string& status,
                                 int code,
                                 const std::string& message,
                                 const rapidjson::Value& data = rapidjson::Value(rapidjson::kObjectType)) {
        rapidjson::Document doc;
        doc.SetObject();
        auto& allocator = doc.GetAllocator();

        doc.AddMember("status", rapidjson::Value(status.c_str(), allocator), allocator);
        doc.AddMember("code", code, allocator);
        doc.AddMember("message", rapidjson::Value(message.c_str(), allocator), allocator);
        doc.AddMember("data", rapidjson::Value(data, allocator), allocator);

        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        doc.Accept(writer);

        res.status = code;
        res.set_content(buffer.GetString(), "application/json");
    }

}
