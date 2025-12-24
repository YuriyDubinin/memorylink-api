#include "uid_generator.h"

namespace utils {
    std::string UIDGenerator::generate() {
        std::lock_guard<std::mutex> lock(get_mutex());

        std::array<unsigned char, 16> bytes{};
        for (auto& b : bytes) {
            b = static_cast<unsigned char>(distribution()(engine()));
        }

        return to_hex(bytes);
    }

    std::string UIDGenerator::to_hex(const std::array<unsigned char, 16>& data) {
        std::ostringstream oss;
        oss << std::hex << std::setfill('0');
        for (auto b : data) {
            oss << std::setw(2) << static_cast<int>(b);
        }
        return oss.str();
    }

    std::mt19937_64& UIDGenerator::engine() {
        static std::mt19937_64 eng(static_cast<unsigned long long>(
            std::chrono::steady_clock::now().time_since_epoch().count()));
        return eng;
    }

    std::uniform_int_distribution<unsigned int>& UIDGenerator::distribution() {
        static std::uniform_int_distribution<unsigned int> dist(0, 255);
        return dist;
    }

    std::mutex& UIDGenerator::get_mutex() {
        static std::mutex mtx;
        return mtx;
    }

} // namespace utils
