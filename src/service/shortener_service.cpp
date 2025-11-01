#include "shortener_service.hpp"
#include "../utils/id_generator.hpp"
#include "../utils/exceptions.hpp"

namespace url_shortener::service {

ShortenerService::ShortenerService(const userver::components::ComponentConfig& config,
                                   const userver::components::ComponentContext& context)
    : userver::components::ComponentBase(config, context),
      repository_(context.FindComponent<db::RepositoryPostgres>()) {}

std::string ShortenerService::CreateShortUrl(const std::string& original_url) {
    if (original_url.empty() || original_url.find("http") != 0)
        throw exceptions::ValidationError("Invalid URL");

    auto short_code = utils::GenerateShortId();
    repository_.InsertUrl(short_code, original_url);
    return short_code;
}

std::string ShortenerService::GetOriginalUrl(const std::string& short_code) {
    auto url = repository_.GetUrl(short_code);
    if (!url)
        throw exceptions::NotFoundError("URL not found");
    return *url;
}

}  // namespace url_shortener::service
