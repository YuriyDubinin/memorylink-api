#include "http_server.hpp"

#include "validators/token.hpp"

HttpServer::HttpServer(const std::string& host, int port) : host_(host), port_(port) {}

void HttpServer::Init() {
    SetupRoutes_();
    Run();
}

void HttpServer::SetupRoutes_() {
    server_.Post("/user/check", [](const httplib::Request& req, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");

        rapidjson::Document body_json;
        std::string error_msg;
        ApiResponse api_response;

        if (!validate::json(req, res, body_json, error_msg)) {
            utils::http_response::send_json_response(res, "ERROR", 400, error_msg);
            return;
        }

        if (!validate::token(body_json, res, error_msg)) {
            utils::http_response::send_json_response(res, "ERROR", 400, error_msg);
            return;
        }

        if (!validate::user_check(body_json, error_msg)) {
            utils::http_response::send_json_response(res, "ERROR", 400, error_msg);
            return;
        }

        utils::http_response::send_json_response(res, "SUCCESS", 200, "Success");
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
