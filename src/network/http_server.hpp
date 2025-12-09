#pragma once

#include <iostream>
#include <string>
#include <httplib/httplib.h>

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
