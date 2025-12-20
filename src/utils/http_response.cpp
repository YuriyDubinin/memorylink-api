#include "http_response.h"

namespace utils::http_response {
    void send(httplib::Response& res, const std::string& status, const int& code,
              const std::string& message, const rapidjson::Value& data) {
        rapidjson::Document doc;
        doc.SetObject();
        auto& allocator = doc.GetAllocator();

        doc.AddMember("status", rapidjson::Value(status.c_str(), allocator), allocator);
        doc.AddMember("code", code, allocator);
        doc.AddMember("message", rapidjson::Value(message.c_str(), allocator), allocator);
        doc.AddMember("data", rapidjson::Value(data, allocator), allocator);

        rapidjson::StringBuffer                    buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        doc.Accept(writer);

        res.status = code;
        res.set_content(buffer.GetString(), "application/json");
    }
} // namespace utils::http_response
