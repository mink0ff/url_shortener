#pragma once

#include <userver/components/component_base.hpp>
#include <userver/storages/postgres/component.hpp>
#include "../db/repository_postgres.hpp"

namespace url_shortener::service {

class ShortenerService final : public userver::components::ComponentBase {
public:
    static constexpr std::string_view kName = "shortener-service";

    ShortenerService(const userver::components::ComponentConfig& config,
                     const userver::components::ComponentContext& context);

    std::string CreateShortUrl(const std::string& original_url);
    std::string GetOriginalUrl(const std::string& short_code);
    std::string DeleteShortUrl(const std::string& short_code);

    static userver::yaml_config::Schema GetStaticConfigSchema();

private:
    db::RepositoryPostgres& repository_;
};

}  // namespace url_shortener::service
