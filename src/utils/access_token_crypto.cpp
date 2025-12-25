#include "access_token_crypto.h"

#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <vector>

#include <openssl/evp.h>
#include <openssl/rand.h>

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

namespace utils::security {
    static std::string to_hex(const std::vector<unsigned char>& data) {
        std::stringstream ss;
        for (auto b : data)
            ss << std::hex << std::setw(2) << std::setfill('0') << (int)b;
        return ss.str();
    }

    static std::vector<unsigned char> from_hex(const std::string& hex) {
        std::vector<unsigned char> data;
        if (hex.size() % 2 != 0)
            throw std::runtime_error("Invalid hex string");
        for (size_t i = 0; i < hex.size(); i += 2) {
            unsigned int      byte;
            std::stringstream ss(hex.substr(i, 2));
            ss >> std::hex >> byte;
            data.push_back(static_cast<unsigned char>(byte));
        }
        return data;
    }

    static std::vector<unsigned char> derive_key(const std::string& key, const std::string& salt) {
        std::vector<unsigned char> out(32);
        if (!EVP_BytesToKey(EVP_aes_256_cbc(),
                            EVP_sha256(),
                            reinterpret_cast<const unsigned char*>(salt.data()),
                            reinterpret_cast<const unsigned char*>(key.data()),
                            key.size(),
                            1,
                            out.data(),
                            nullptr)) {
            throw std::runtime_error("Key derivation failed");
        }
        return out;
    }

    static std::string
    encrypt_bytes(const std::string& plaintext, const std::string& key, const std::string& salt) {
        auto                       derived_key = derive_key(key, salt);
        std::vector<unsigned char> iv(EVP_MAX_IV_LENGTH);
        if (!RAND_bytes(iv.data(), iv.size()))
            throw std::runtime_error("Failed to generate IV");

        EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
        if (!ctx)
            throw std::runtime_error("Failed to create context");

        if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, derived_key.data(), iv.data()))
            throw std::runtime_error("EncryptInit failed");

        std::vector<unsigned char> ciphertext(plaintext.size() +
                                              EVP_CIPHER_block_size(EVP_aes_256_cbc()));
        int                        len;
        if (1 != EVP_EncryptUpdate(ctx,
                                   ciphertext.data(),
                                   &len,
                                   reinterpret_cast<const unsigned char*>(plaintext.data()),
                                   plaintext.size()))
            throw std::runtime_error("EncryptUpdate failed");
        int ciphertext_len = len;

        if (1 != EVP_EncryptFinal_ex(ctx, ciphertext.data() + len, &len))
            throw std::runtime_error("EncryptFinal failed");
        ciphertext_len += len;

        EVP_CIPHER_CTX_free(ctx);
        ciphertext.resize(ciphertext_len);

        std::vector<unsigned char> out = iv;
        out.insert(out.end(), ciphertext.begin(), ciphertext.end());
        return to_hex(out);
    }

    static std::string
    decrypt_bytes(const std::string& hex_input, const std::string& key, const std::string& salt) {
        auto data = from_hex(hex_input);
        if (data.size() < EVP_MAX_IV_LENGTH)
            throw std::runtime_error("Invalid data size");

        auto                       derived_key = derive_key(key, salt);
        std::vector<unsigned char> iv(data.begin(), data.begin() + EVP_MAX_IV_LENGTH);
        std::vector<unsigned char> ciphertext(data.begin() + EVP_MAX_IV_LENGTH, data.end());

        EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
        if (!ctx)
            throw std::runtime_error("Failed to create context");

        if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, derived_key.data(), iv.data()))
            throw std::runtime_error("DecryptInit failed");

        std::vector<unsigned char> plaintext(ciphertext.size() +
                                             EVP_CIPHER_block_size(EVP_aes_256_cbc()));
        int                        len;
        if (1 !=
            EVP_DecryptUpdate(ctx, plaintext.data(), &len, ciphertext.data(), ciphertext.size()))
            throw std::runtime_error("DecryptUpdate failed");
        int plaintext_len = len;

        if (1 != EVP_DecryptFinal_ex(ctx, plaintext.data() + len, &len))
            throw std::runtime_error("DecryptFinal failed");
        plaintext_len += len;

        EVP_CIPHER_CTX_free(ctx);
        plaintext.resize(plaintext_len);

        return std::string(plaintext.begin(), plaintext.end());
    }

    std::string encrypt_access_token_struct(const AccessTokenData& token,
                                            const std::string&     key,
                                            const std::string&     salt) {
        rapidjson::Document d;
        d.SetObject();
        auto& allocator = d.GetAllocator();
        d.AddMember("id", token.id, allocator);
        d.AddMember("family_id", token.family_id, allocator);
        d.AddMember("ttl", token.ttl, allocator);
        d.AddMember("role", rapidjson::Value(token.role.c_str(), allocator), allocator);

        rapidjson::StringBuffer                    buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        d.Accept(writer);

        return encrypt_bytes(buffer.GetString(), key, salt);
    }

    AccessTokenData decrypt_access_token_struct(const std::string& encrypted_hex,
                                                const std::string& key,
                                                const std::string& salt) {
        std::string json_str = decrypt_bytes(encrypted_hex, key, salt);

        rapidjson::Document d;
        d.Parse(json_str.c_str());
        if (d.HasParseError())
            throw std::runtime_error("Failed to parse decrypted JSON");

        AccessTokenData token;
        token.id        = d["id"].GetInt64();
        token.family_id = d["family_id"].GetInt64();
        token.ttl       = d["ttl"].GetInt64();
        token.role      = d["role"].GetString();

        return token;
    }

} // namespace utils::security
