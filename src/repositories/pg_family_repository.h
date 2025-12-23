#pragma once

#include <optional>
#include <stdexcept>
#include <string>

#include <db/postgres_connection.hpp>
#include <models/family.h>
#include <utils/format_pg_timestamp.h>

#include "family_repository.h"

class PgFamilyRepository : public FamilyRepository {
public:
    explicit PgFamilyRepository(PostgresConnection& db_conn);
    ~PgFamilyRepository() override = default;

    std::optional<Family> GetById(std::int64_t family_id) override;

private:
    PostgresConnection& db_;

    FamilyStatus ParseStatus_(const std::string& status_str);
};