#pragma once

#include <memory>

#include "postgres_connection.hpp"
#include <repositories/pg_user_repository.h>

class DBRegistry {
public:
    static void Init(PostgresConnection& db) {
        user_repo_ = std::make_unique<PgUserRepository>(db);
    }

    static PgUserRepository& Users() {
        if (!user_repo_)
            throw std::runtime_error("User repository not initialized");
        return *user_repo_;
    }

private:
    static std::unique_ptr<PgUserRepository> user_repo_;
};
