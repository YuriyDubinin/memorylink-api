#pragma once

#include <string>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <vector>

#include <openssl/evp.h>
#include <openssl/crypto.h>

namespace utils::security {
    std::string hash_password(const std::string& login,
                              const std::string& password);

    bool verify_password(const std::string& login,
                         const std::string& password,
                         const std::string& stored_hash);

} // namespace utils::security
