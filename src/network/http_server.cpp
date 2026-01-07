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
    // User
    server_.Get("/user", [](const httplib::Request& req, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");

        rapidjson::Document body_json;
        ApiResponse         api_response;

        // Validation
        if (!validate::json_request(req, res, body_json, api_response) ||
            !validate::get_user_by_id(req, body_json, api_response)) {
            utils::http_response::send(res, api_response);
            return;
        }

        UserService user_service(res, body_json, api_response);
        user_service.GetById();
    });

    server_.Post("/user/auth", [](const httplib::Request& req, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");

        rapidjson::Document body_json;
        ApiResponse         api_response;

        // Validation
        if (!validate::json(req, body_json, api_response) ||
            !validate::auth_user(body_json, api_response)) {
            utils::http_response::send(res, api_response);
            return;
        }

        UserService user_service(res, body_json, api_response);
        user_service.Auth();
    });

    // Family
    server_.Get("/family", [](const httplib::Request& req, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");

        rapidjson::Document body_json;
        ApiResponse         api_response;

        // Validation
        if (!validate::json_request(req, res, body_json, api_response) ||
            !validate::get_family_by_id(req, body_json, api_response)) {
            utils::http_response::send(res, api_response);
            return;
        }

        FamilyService family_service(res, body_json, api_response);
        family_service.GetById();
    });

    // Photo
    server_.Get("/photo", [](const httplib::Request& req, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");

        rapidjson::Document body_json;
        ApiResponse         api_response;

        // Validation
        if (!validate::json_request(req, res, body_json, api_response) ||
            !validate::get_photo_by_id(body_json, api_response)) {
            return;
        }

        PhotoService photo_service(req, res, body_json, api_response);
        photo_service.GetById();
    });

    server_.Get("/photo/list", [](const httplib::Request& req, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");

        rapidjson::Document body_json;
        ApiResponse         api_response;

        // Validation
        if (!validate::json_request(req, res, body_json, api_response) ||
            !validate::get_photo_list_by_family_id(req, body_json, api_response)) {
            utils::http_response::send(res, api_response);
            return;
        }

        PhotoService photo_service(req, res, body_json, api_response);
        photo_service.GetListByFamilyId();
    });

    server_.Post("/photo/list/upload", [](const httplib::Request& req, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");

        rapidjson::Document body_json;
        ApiResponse         api_response;

        // Validation
        if (!validate::file_request(req, api_response) ||
            !validate::upload_photo_list_by_family_id(req, body_json, api_response)) {
            utils::http_response::send(res, api_response);
            return;
        }

        PhotoService photo_service(req, res, body_json, api_response);
        photo_service.UploadListByFamilyId();
    });

    // Video
    server_.Get("/video", [](const httplib::Request& req, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");

        rapidjson::Document body_json;
        ApiResponse         api_response;

        // Validation
        if (!validate::json_request(req, res, body_json, api_response) ||
            !validate::get_video_by_id(body_json, api_response)) {
            utils::http_response::send(res, api_response);
            return;
        }

        VideoService video_service(req, res, body_json, api_response);
        video_service.GetById();
    });

    server_.Get("/video/list", [](const httplib::Request& req, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");

        rapidjson::Document body_json;
        ApiResponse         api_response;

        // Validation
        if (!validate::json_request(req, res, body_json, api_response) ||
            !validate::get_video_list_by_family_id(req, body_json, api_response)) {
            utils::http_response::send(res, api_response);
            return;
        }

        VideoService video_service(req, res, body_json, api_response);
        video_service.GetListByFamilyId();
    });

    server_.Post("/video/upload", [](const httplib::Request& req, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");

        rapidjson::Document body_json;
        ApiResponse         api_response;

        // Validation
        if (!validate::file_request(req, api_response) ||
            !validate::upload_video_list_by_family_id(req, body_json, api_response)) {
            utils::http_response::send(res, api_response);
            return;
        }

        VideoService video_service(req, res, body_json, api_response);
        video_service.UploadByFamilyId();
    });

    SetupRoutesOptions_(server_, routes_);
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
