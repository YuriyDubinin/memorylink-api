#include "http_server.h"

HttpServer::HttpServer(const std::string& host, int port) : host_(host), port_(port) {}

void HttpServer::Init() {
    SetupRoutes_();
    Run();
}

void HttpServer::Run() {
    std::cout << "[HTTPServer]: started, " << host_ << ":" << port_ << std::endl;
    server_.listen(host_, port_);
}

void HttpServer::SetupRoutes_() {
    const std::vector<std::string> routes = {"/user/auth", "/user"};

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
        user_service.GetById();
    });

    server_.Get("/family", [](const httplib::Request& req, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");

        rapidjson::Document body_json;
        ApiResponse         api_response;

        // Validation
        if (!validate::default_request(req, res, body_json, api_response)) {
            return;
        }

        if (!validate::family_get(body_json, api_response)) {
            utils::http_response::send(res, api_response);
            return;
        }

        FamilyService family_service(res, body_json, api_response);
        family_service.GetById();
    });

    SetupRoutesOptions_(server_, routes);
}

void HttpServer::SetupRoutesOptions_(httplib::Server&                server,
                                     const std::vector<std::string>& routes) {
    for (const auto& route : routes) {
        server.Options(route, [](const httplib::Request&, httplib::Response& res) {
            res.set_header("Access-Control-Allow-Origin", "*");
            res.set_header("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
            res.set_header("Access-Control-Allow-Headers", "Content-Type, Authorization");
            res.status = 204;
        });
    }
}
