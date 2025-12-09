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
    } catch (const std::exception& ex) {
        std::cerr << "[MAIN]: ERROR: " << ex.what() << std::endl;
        return 1;
    }
    return 0;
}
