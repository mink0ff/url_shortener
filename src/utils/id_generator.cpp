#include "id_generator.hpp"
#include <userver/utils/uuid4.hpp>

namespace url_shortener::utils {

std::string GenerateShortId() {
    auto uuid = userver::utils::generators::GenerateUuid();
    return uuid.substr(0, 8); 

}  

}   // namespace url_shortener::utils