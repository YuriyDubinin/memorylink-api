#include "http_server.hpp"

HttpServer::HttpServer(const std::string& host, int port)
    : host_(host), port_(port) {}

void HttpServer::Init() {
    SetupRoutes_();
    Run();
    std::cout << "[HTTPServer]: started, host:port: "
              << host_ << ":" << port_ << std::endl;
}

void HttpServer::SetupRoutes_() {
    server_.Get("/test", [](const httplib::Request&, httplib::Response& res){
        res.set_content("Test", "text/plain");
    });
}

void HttpServer::Run() {
    server_.listen(host_, port_);
}
