#include "http_server.hpp"

HttpServer::HttpServer(const std::string& host, int port)
    : host_(host), port_(port) {}

void HttpServer::Init() {
    SetupRoutes_();
    Run();
}

void HttpServer::SetupRoutes_() {
    server_.Get("/test", [](const httplib::Request&, httplib::Response& res){
        utils::http_response::SendJsonResponse(res, "Unauthorized", 401, "Authentication required");
    });
}


void HttpServer::Run() {
    std::cout << "[HTTPServer]: started, "
              << host_ << ":" << port_ << std::endl;
    server_.listen(host_, port_);
}
