#include <iostream>

#include <config/config_loader.h>
#include <db/db_registry.h>
#include <db/postgres_connection.hpp>
#include <models/config.h>
#include <network/http_server.h>

int main() {
    try {
        const Config       cfg = ConfigLoader::Load("config.json");
        PostgresConnection db(cfg.db);
        HttpServer         server(cfg.host, cfg.port);

        db.Check();
        DBRegistry::Init(db);

        server.Init();
    } catch (const std::exception& ex) {
        std::cerr << "[MAIN]: ERROR: " << ex.what() << std::endl;
        return 1;
    }
    return 0;
}
