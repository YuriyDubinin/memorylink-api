#pragma once

#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

#include <db/postgres_connection.hpp>
#include <models/video.h>
#include <utils/format_pg_timestamp.h>

#include "video_repository.h"

class PgVideoRepository : public VideoRepository {
public:
    explicit PgVideoRepository(PostgresConnection& db_conn);
    ~PgVideoRepository() override = default;

    std::optional<Video> GetById(std::int64_t video_id) override;
    VideoListResult
    GetListByFamilyId(std::int64_t family_id, std::size_t limit, std::size_t offset);

    void InsertListByFamilyId(std::int64_t family_id, std::vector<Video>& video_list);

private:
    PostgresConnection& db_;
};