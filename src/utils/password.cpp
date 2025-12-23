#include "password.h"

namespace utils::security {
    namespace {
        constexpr int ITERATIONS = 200'000;
        constexpr int HASH_LEN   = 32; // 256 бит

        std::string to_hex(const unsigned char* data, size_t len) {
            std::ostringstream oss;
            oss << std::hex << std::setfill('0');

            for (size_t i = 0; i < len; ++i) {
                oss << std::setw(2) << static_cast<int>(data[i]);
            }
            return oss.str();
        }

        std::vector<unsigned char> pbkdf2(const std::string& password, const std::string& salt) {
            std::vector<unsigned char> hash(HASH_LEN);

            if (PKCS5_PBKDF2_HMAC(password.data(),
                                  static_cast<int>(password.size()),
                                  reinterpret_cast<const unsigned char*>(salt.data()),
                                  static_cast<int>(salt.size()),
                                  ITERATIONS,
                                  EVP_sha256(),
                                  HASH_LEN,
                                  hash.data()) != 1) {
                throw std::runtime_error("PBKDF2_HMAC failed");
            }

            return hash;
        }

        bool constant_time_equals(const std::string& a, const std::string& b) {
            if (a.size() != b.size())
                return false;

            return CRYPTO_memcmp(a.data(), b.data(), a.size()) == 0;
        }
    } // namespace

    std::string hash_password(const std::string& email, const std::string& password) {
        const auto hash = pbkdf2(password, email);
        return to_hex(hash.data(), hash.size());
    }

    bool verify_password(const std::string& email,
                         const std::string& password,
                         const std::string& stored_hash) {
        const std::string computed = hash_password(email, password);
        return constant_time_equals(computed, stored_hash);
    }

} // namespace utils::security
