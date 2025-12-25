#include "extract_bearer_token.h"

namespace utils {
    std::optional<std::string> extract_bearer_token(const httplib::Request& req) {
        auto it = req.headers.find("Authorization");
        if (it == req.headers.end())
            return std::nullopt;

        const std::string& auth_header = it->second;
        const std::string  bearer      = "Bearer ";
        if (auth_header.size() <= bearer.size())
            return std::nullopt;
        if (auth_header.substr(0, bearer.size()) != bearer)
            return std::nullopt;

        return auth_header.substr(bearer.size());
    }

} // namespace utils
