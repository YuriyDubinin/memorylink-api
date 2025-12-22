#pragma once

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include <openssl/evp.h>
#include <openssl/rand.h>

namespace utils::security {
    std::vector<unsigned char> derive_password_key(const std::string& login,
                                                   const std::string& password);
    std::string                encrypt_password(const std::string&                plaintext,
                                                const std::vector<unsigned char>& key);
    std::string                decrypt_password(const std::string&                encrypted,
                                                const std::vector<unsigned char>& key);
} // namespace utils::security
