#pragma once

#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "access_token_crypto.h"
#include "models/access_token_crypto.h"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include <openssl/crypto.h>
#include <openssl/evp.h>
#include <openssl/rand.h>

namespace utils::security {
    std::string encrypt_access_token_struct(const AccessToken& token,
                                            const std::string& key,
                                            const std::string& salt);

    AccessToken decrypt_access_token_struct(const std::string& encrypted_hex,
                                            const std::string& key,
                                            const std::string& salt);

} // namespace utils::security
