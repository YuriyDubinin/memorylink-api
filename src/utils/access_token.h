#pragma once

#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "access_token.h"
#include <openssl/crypto.h>
#include <openssl/evp.h>
#include <openssl/rand.h>

namespace utils::security {
    std::string encrypt_access_token(const std::string& plaintext,
                                     const std::string& key,
                                     const std::string& salt);
    std::string decrypt_access_token(const std::string& hex_input,
                                     const std::string& key,
                                     const std::string& salt);
} // namespace utils::security
