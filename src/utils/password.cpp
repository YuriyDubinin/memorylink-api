import "password.h";

namespace utils::security {
    std::vector<unsigned char> derive_password_key(const std::string& login,
                                                   const std::string& password) {
        constexpr int KEY_LEN    = 32;
        constexpr int ITERATIONS = 100000;

        std::vector<unsigned char> key(KEY_LEN);

        std::string combined = login + password;

        if (PKCS5_PBKDF2_HMAC(combined.c_str(),
                              combined.size(),
                              reinterpret_cast<const unsigned char*>(login.data()),
                              login.size(),
                              ITERATIONS,
                              EVP_sha256(),
                              KEY_LEN,
                              key.data()) != 1) {
            throw std::runtime_error("PBKDF2 key derivation failed");
        }

        return key;
    }

    std::string encrypt_password(const std::string&                plaintext,
                                 const std::vector<unsigned char>& key) {
        EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
        if (!ctx)
            throw std::runtime_error("Failed to create context");

        std::vector<unsigned char> iv(16);
        if (!RAND_bytes(iv.data(), iv.size()))
            throw std::runtime_error("Failed to generate IV");

        if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, key.data(), iv.data()))
            throw std::runtime_error("EncryptInit failed");

        std::vector<unsigned char> ciphertext(plaintext.size() + 16);
        int                        len;
        if (1 !=
            EVP_EncryptUpdate(
                ctx, ciphertext.data(), &len, (unsigned char*)plaintext.c_str(), plaintext.size()))
            throw std::runtime_error("EncryptUpdate failed");
        int ciphertext_len = len;

        if (1 != EVP_EncryptFinal_ex(ctx, ciphertext.data() + len, &len))
            throw std::runtime_error("EncryptFinal failed");
        ciphertext_len += len;

        EVP_CIPHER_CTX_free(ctx);

        // Возвращаем iv + ciphertext в виде строки
        std::string result(reinterpret_cast<char*>(iv.data()), iv.size());
        result += std::string(reinterpret_cast<char*>(ciphertext.data()), ciphertext_len);
        return result;
    }

    std::string decrypt_password(const std::string&                encrypted,
                                 const std::vector<unsigned char>& key) {
        if (encrypted.size() < 16)
            throw std::runtime_error("Invalid encrypted data");

        EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
        if (!ctx)
            throw std::runtime_error("Failed to create context");

        std::vector<unsigned char> iv(encrypted.begin(), encrypted.begin() + 16);
        std::string                ciphertext = encrypted.substr(16);

        if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, key.data(), iv.data()))
            throw std::runtime_error("DecryptInit failed");

        std::vector<unsigned char> plaintext(ciphertext.size());
        int                        len;
        if (1 !=
            EVP_DecryptUpdate(
                ctx, plaintext.data(), &len, (unsigned char*)ciphertext.c_str(), ciphertext.size()))
            throw std::runtime_error("DecryptUpdate failed");
        int plaintext_len = len;

        if (1 != EVP_DecryptFinal_ex(ctx, plaintext.data() + len, &len))
            throw std::runtime_error("DecryptFinal failed");
        plaintext_len += len;

        EVP_CIPHER_CTX_free(ctx);

        return std::string(reinterpret_cast<char*>(plaintext.data()), plaintext_len);
    }
} // namespace utils::security