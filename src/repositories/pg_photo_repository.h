#pragma once

#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

#include <db/postgres_connection.hpp>
#include <models/photo.h>
#include <utils/format_pg_timestamp.h>

#include "photo_repository.h"

class PgPhotoRepository : public PhotoRepository {
public:
    explicit PgPhotoRepository(PostgresConnection& db_conn);
    ~PgPhotoRepository() override = default;

    std::optional<Photo> GetById(std::int64_t photo_id) override;
    PhotoListResult
    GetListByFamilyId(std::int64_t family_id, std::size_t limit, std::size_t offset);

    void InsertListByFamilyId(std::int64_t family_id, std::vector<Photo>& photo_list);

private:
    PostgresConnection& db_;
};