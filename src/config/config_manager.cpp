#include "config/config_manager.h"

#include <cstdio>
#include <iostream>
#include <stdexcept>

#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"

void ConfigManager::Load(const std::string& filename) {
    instance_().LoadConfig_(filename);
}

const Config& ConfigManager::Get() {
    return instance_().cfg_;
}

ConfigManager& ConfigManager::instance_() {
    static ConfigManager mgr;
    return mgr;
}

void ConfigManager::LoadConfig_(const std::string& filename) {
    FILE* fp = fopen(filename.c_str(), "r");
    if (!fp) {
        throw std::runtime_error("[ConfigManager]: Cannot open config file: " + filename);
    }

    char                      readBuffer[65536];
    rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));

    rapidjson::Document doc;
    doc.ParseStream(is);
    fclose(fp);

    if (doc.HasParseError()) {
        throw std::runtime_error("[ConfigManager]: JSON parse error in config file");
    }

    // HTTP
    if (doc.HasMember("http") && doc["http"].IsObject()) {
        const auto& http = doc["http"];
        if (http.HasMember("host") && http["host"].IsString()) {
            cfg_.host = http["host"].GetString();
        } else {
            throw std::runtime_error("[ConfigManager]: Missing or invalid 'host' in config");
        }

        if (http.HasMember("port") && http["port"].IsInt()) {
            cfg_.port = http["port"].GetInt();
        } else {
            throw std::runtime_error("[ConfigManager]: Missing or invalid 'port' in config");
        }
    } else {
        throw std::runtime_error("[ConfigManager]: Missing 'http' section in config");
    }

    // DB
    if (doc.HasMember("db") && doc["db"].IsString()) {
        cfg_.db = doc["db"].GetString();
    } else {
        throw std::runtime_error("[ConfigManager]: Missing or invalid 'db' in config");
    }

    // S3
    if (doc.HasMember("s3") && doc["s3"].IsObject()) {
        const auto& s3 = doc["s3"];
        if (s3.HasMember("access_key") && s3["access_key"].IsString()) {
            cfg_.s3_access_key = s3["access_key"].GetString();
        } else {
            throw std::runtime_error("[ConfigManager]: Missing or invalid 'access_key' in config");
        }

        if (s3.HasMember("secret_key") && s3["secret_key"].IsString()) {
            cfg_.s3_secret_key = s3["secret_key"].GetString();
        } else {
            throw std::runtime_error("[ConfigManager]: Missing or invalid 'secret_key' in config");
        }

        if (s3.HasMember("endpoint") && s3["endpoint"].IsString()) {
            cfg_.s3_endpoint = s3["endpoint"].GetString();
        } else {
            throw std::runtime_error("[ConfigManager]: Missing or invalid 'endpoint' in config");
        }

        if (s3.HasMember("region") && s3["region"].IsString()) {
            cfg_.s3_region = s3["region"].GetString();
        } else {
            throw std::runtime_error("[ConfigManager]: Missing or invalid 'region' in config");
        }

    } else {
        throw std::runtime_error("[ConfigManager]: Missing 'http' section in config");
    }

    // Pepper
    if (doc.HasMember("pepper") && doc["pepper"].IsString()) {
        cfg_.pepper = doc["pepper"].GetString();
    } else {
        throw std::runtime_error("[ConfigManager]: Missing or invalid 'pepper' in config");
    }

    // Salt
    if (doc.HasMember("salt") && doc["salt"].IsString()) {
        cfg_.salt = doc["salt"].GetString();
    } else {
        throw std::runtime_error("[ConfigManager]: Missing or invalid 'salt' in config");
    }

    std::cout << "[ConfigManager]: Config successfully loaded" << std::endl;
}
