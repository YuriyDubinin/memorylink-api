#pragma once

#include <iostream>
#include <string>
#include <vector>

#include <httplib/httplib.h>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include <models/api_response.h>
#include <services/family_service.h>
#include <services/user_service.h>
#include <utils/http_response.h>
#include <validators/access_token.h>
#include <validators/default_request.h>
#include <validators/family_get.h>
#include <validators/json.h>
#include <validators/user_auth.h>
#include <validators/user_get.h>

class HttpServer {
public:
    explicit HttpServer(const std::string& host, int port = 8080);
    void Init();
    void Run();

private:
    httplib::Server server_;
    std::string     host_;
    int             port_;

    void SetupRoutes_();
    void SetupRoutesOptions_(httplib::Server& server, const std::vector<std::string>& routes);
};
