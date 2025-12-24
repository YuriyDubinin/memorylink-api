#include <iostream>

#include <config/config_manager.h>
#include <db/db_registry.h>
#include <db/postgres_connection.hpp>
#include <models/config.h>
#include <network/http_server.h>

int main() {
    try {
        ConfigManager::Load("config.json");
        const Config& cfg = ConfigManager::Get();

        PostgresConnection db(cfg.db);
        db.Check();
        DBRegistry::Init(db);

        HttpServer http_server(cfg.host, cfg.port);
        http_server.Init();
    } catch (const std::exception& ex) {
        std::cerr << "[MAIN]: ERROR: " << ex.what() << std::endl;
        return 1;
    }
    return 0;
}
