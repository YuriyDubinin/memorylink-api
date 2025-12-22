#include "db_registry.h"

// Определение статического члена
std::unique_ptr<PgUserRepository> DBRegistry::user_repo_ = nullptr;