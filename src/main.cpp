#include <iostream>
#include <network/http_server.hpp>
#include <config/config_loader.hpp>
#include <network/http_server.hpp>
#include <models/config.hpp>

int main() {
    try {
        Config cfg = ConfigLoader::Load("config.json");

        HttpServer server(cfg.host, cfg.port);
        server.Init();

        std::cout << "[MAIN]: Server running at: " << cfg.host << ":" << cfg.port
        << ", with upload path: " << cfg.upload_path << std::endl;
    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
        return 1;
    }
    return 0;
}
