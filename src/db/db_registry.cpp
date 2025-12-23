#include "db_registry.h"

// Определение статического члена
std::unique_ptr<PgUserRepository>   DBRegistry::user_repo_   = nullptr;
std::unique_ptr<PgFamilyRepository> DBRegistry::family_repo_ = nullptr;
std::unique_ptr<PgPhotoRepository>  DBRegistry::photo_repo_  = nullptr;
std::unique_ptr<PgVideoRepository>  DBRegistry::video_repo_  = nullptr;