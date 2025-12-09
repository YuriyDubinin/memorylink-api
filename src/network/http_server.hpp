#pragma once

#include <iostream>
#include <httplib/httplib.h>

class HttpServer {
public:
    HttpServer(const int port = 8080);
    void Init();
    void Run();
private:
    httplib::Server server_;
    int port_;
    void SetupRoutes_();
};