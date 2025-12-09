#include <config/config_loader.hpp>
#include <iostream> // для std::cout

Config ConfigLoader::Load(const std::string& filename) {
    FILE* fp = fopen(filename.c_str(), "r");
    if (!fp) {
        throw std::runtime_error("Cannot open config file: " + filename);
    }

    char readBuffer[65536];
    rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));

    rapidjson::Document doc;
    doc.ParseStream(is);
    fclose(fp);

    if (doc.HasParseError()) {
        throw std::runtime_error("JSON parse error in config file");
    }

    Config cfg;

    // Парс http
    if (doc.HasMember("http") && doc["http"].IsObject()) {
        const auto& http = doc["http"];
        if (http.HasMember("host") && http["host"].IsString()) {
            cfg.host = http["host"].GetString();
        } else {
            throw std::runtime_error("Missing or invalid 'host' in config");
        }

        if (http.HasMember("port") && http["port"].IsInt()) {
            cfg.port = doc["http"]["port"].GetInt();
        } else {
            throw std::runtime_error("Missing or invalid 'port' in config");
        }
    } else {
        throw std::runtime_error("Missing 'http' section in config");
    }

    // Парс пути загрузки файлов
    if (doc.HasMember("upload_path") && doc["upload_path"].IsString()) {
        cfg.upload_path = doc["upload_path"].GetString();
    } else {
        cfg.upload_path = "uploads/"; // по умолчанию
    }

    // Логирование успешной загрузки
    std::cout << "[ConfigLoader]: Config successfully loaded" << std::endl;

    return cfg;
}
