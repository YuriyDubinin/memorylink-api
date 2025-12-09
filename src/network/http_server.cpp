#include "http_server.hpp"

HttpServer::HttpServer(const int port) : port_(port) {}

void HttpServer::Init() {
    SetupRoutes_();
    Run();
    std::cout << "[HTTP Server]: started" << std::endl;
}

void HttpServer::SetupRoutes_() {
    server_.Get("/test", [](const httplib::Request&, httplib::Response& res){
        res.set_content("Hello from /test!", "text/plain");
    });
}

void HttpServer::Run() {
    server_.listen("0.0.0.0", port_);
}