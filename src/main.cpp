#include <iostream>
#include <network/http_server.hpp>
#include <config/config_loader.hpp>
#include <db/postgres_connection.h>
#include <models/config.hpp>

int main() {
    try {
        const Config cfg = ConfigLoader::Load("config.json");

        PostgresConnection db(cfg.db);
        if (!db.IsConnected()) {
            std::cerr << "[MAIN]: ERROR: DB is not connected" << std::endl;
        }
        std::cout << "[MAIN]: DB connected successfully" << std::endl;

        HttpServer server(cfg.host, cfg.port);
        server.Init();
    } catch (const std::exception& ex) {
        std::cerr << "[MAIN]: ERROR: " << ex.what() << std::endl;
        return 1;
    }
    return 0;
}
