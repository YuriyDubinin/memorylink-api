#pragma once

#include <optional>
#include <string>

#include <db/postgres_connection.hpp>
#include <models/user.h>
#include <utils/format_pg_timestamp.h>

#include "user_repository.h"

class PgUserRepository : public UserRepository {
public:
    explicit PgUserRepository(PostgresConnection& db_conn);
    ~PgUserRepository() override = default;

    std::optional<User> GetById(std::int64_t user_id) override;
    std::optional<User> GetByEmail(const std::string& email) override;

private:
    PostgresConnection& db_;

    UserStatus ParseStatus_(const std::string& status_str);
    UserRole   ParseRole_(const std::string& role_str);
};
