#pragma once

#include <httplib/httplib.h>
#include <iostream>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <string>
#include <utils/http_response.h>
#include <utils/validate_json.hpp>
#include <utils/check_token.hpp>
#include <validators/user.hpp>

class HttpServer {
public:
    explicit HttpServer(const std::string& host, int port = 8080);
    void Init();
    void Run();

private:
    void SetupRoutes_();

    httplib::Server server_;
    std::string host_;
    int port_;
};
