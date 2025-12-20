#pragma once

#include <string>

struct ApiResponse {
    std::string status = "SUCCESS";
    int         code = 200;
    std::string msg = "Success";
};