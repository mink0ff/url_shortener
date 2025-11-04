#include <regex>
#include "validation.hpp"

namespace url_shortener::utils {

bool IsValidUrl(const std::string& url) {
    static const std::regex url_pattern(R"(^(http|https)://[a-zA-Z0-9\.-]+\.[a-z]{2,}.*$)");
    return std::regex_match(url, url_pattern);
}

}  // namespace url_shortener::utils
