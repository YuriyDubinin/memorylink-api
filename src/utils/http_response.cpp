#include "http_response.h"

namespace utils::http_response {
    void send(httplib::Response& res, ApiResponse& api_response, const rapidjson::Value& data) {
        rapidjson::Document doc;
        doc.SetObject();
        auto& allocator = doc.GetAllocator();

        doc.AddMember(
            "status", rapidjson::Value(api_response.status.c_str(), allocator), allocator);
        doc.AddMember("code", api_response.code, allocator);
        doc.AddMember("message", rapidjson::Value(api_response.msg.c_str(), allocator), allocator);
        doc.AddMember("data", rapidjson::Value(data, allocator), allocator);

        rapidjson::StringBuffer                    buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        doc.Accept(writer);

        res.status = api_response.code;
        res.set_content(buffer.GetString(), "application/json");
    }
} // namespace utils::http_response
