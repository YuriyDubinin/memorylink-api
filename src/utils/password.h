#pragma once

#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include <openssl/crypto.h>
#include <openssl/evp.h>

namespace utils::security {
    std::string hash_password(const std::string& login, const std::string& password);

    bool verify_password(const std::string& login,
                         const std::string& password,
                         const std::string& stored_hash);

} // namespace utils::security
