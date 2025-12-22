#include "http_server.h"

HttpServer::HttpServer(const std::string& host, int port) : host_(host), port_(port) {}

void HttpServer::Init() {
    SetupRoutes_();
    Run();
}

void HttpServer::SetupRoutes_() {
    server_.Post("/user/auth", [](const httplib::Request& req, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");

        rapidjson::Document body_json;
        ApiResponse         api_response;

        // Validation
        if (!validate::json(req, body_json, api_response)) {
            return;
        }

        if (!validate::user_auth(body_json, api_response)) {
            utils::http_response::send(res, api_response);
            return;
        }

        UserService user_service(res, body_json, api_response);
        user_service.Auth();
    });

    server_.Get("/user", [](const httplib::Request& req, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");

        rapidjson::Document body_json;
        ApiResponse         api_response;

        // Validation
        if (!validate::default_request(req, res, body_json, api_response)) {
            return;
        }

        if (!validate::user_get(body_json, api_response)) {
            utils::http_response::send(res, api_response);
            return;
        }

        UserService user_service(res, body_json, api_response);
        user_service.Get();
    });

    // Options
    server_.Options("/user/check", [](const httplib::Request&, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type");
        res.status = 204;
    });
}

void HttpServer::Run() {
    std::cout << "[HTTPServer]: started, " << host_ << ":" << port_ << std::endl;
    server_.listen(host_, port_);
}
