#pragma once

#include <array>
#include <chrono>
#include <iomanip>
#include <mutex>
#include <random>
#include <sstream>
#include <string>

namespace utils {
    class UIDGenerator {
    public:
        static std::string generate(); // hex

    private:
        static std::string to_hex(const std::array<unsigned char, 16>& data);

        static std::mt19937_64&                             engine();
        static std::uniform_int_distribution<unsigned int>& distribution();
        static std::mutex&                                  get_mutex();
    };

} // namespace utils
