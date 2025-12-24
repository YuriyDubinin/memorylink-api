#pragma once

#include <string>

struct Config {
    int         port;
    std::string host;
    std::string db;
    std::string pepper;
    std::string salt;
    std::string s3_access_key;
    std::string s3_secret_key;
    std::string s3_endpoint;
    std::string s3_region;
};