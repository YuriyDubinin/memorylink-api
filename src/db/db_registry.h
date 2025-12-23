#pragma once

#include <memory>

#include <repositories/pg_family_repository.h>
#include <repositories/pg_user_repository.h>

#include "postgres_connection.hpp"

class DBRegistry {
public:
    static void Init(PostgresConnection& db) {
        user_repo_   = std::make_unique<PgUserRepository>(db);
        family_repo_ = std::make_unique<PgFamilyRepository>(db);
    }

    static PgUserRepository& UserRepository() {
        if (!user_repo_)
            throw std::runtime_error("User repository not initialized");
        return *user_repo_;
    }

    static PgFamilyRepository& FamilyRepository() {
        if (!family_repo_)
            throw std::runtime_error("Family repository not initialized");
        return *family_repo_;
    }

private:
    static std::unique_ptr<PgUserRepository>   user_repo_;
    static std::unique_ptr<PgFamilyRepository> family_repo_;
};
