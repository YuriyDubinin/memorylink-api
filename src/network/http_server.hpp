#pragma once

#include <iostream>
#include <string>
#include <httplib/httplib.h>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <validators/user.hpp>
#include <utils/http_response.h>

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
