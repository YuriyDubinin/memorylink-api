#pragma once

#include <optional>
#include <stdexcept>
#include <string>

#include <db/postgres_connection.hpp>
#include <models/photo.h>
#include <utils/format_pg_timestamp.h>

#include "photo_repository.h"

class PgPhotoRepository : public PhotoRepository {
public:
    explicit PgPhotoRepository(PostgresConnection& db_conn);
    ~PgPhotoRepository() override = default;

    std::optional<Photo> GetById(std::int64_t family_id) override;

private:
    PostgresConnection& db_;
};