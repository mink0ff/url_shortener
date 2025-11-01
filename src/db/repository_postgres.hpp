#pragma once

#include <userver/components/component_base.hpp>
#include <userver/storages/postgres/component.hpp>
#include <userver/storages/postgres/cluster.hpp>
#include <optional>
#include <string>

namespace url_shortener::db {

class RepositoryPostgres final : public userver::components::ComponentBase {
public:
    static constexpr std::string_view kName = "repository-postgres";

    explicit RepositoryPostgres(const userver::components::ComponentConfig& config,
                       const userver::components::ComponentContext& context);

    std::optional<std::string> InsertUrl(const std::string& short_url,
                                         const std::string& original_url);

    std::optional<std::string> GetUrl(const std::string& short_url);

    static userver::yaml_config::Schema GetStaticConfigSchema();

private:
    userver::storages::postgres::ClusterPtr pg_cluster_;
};

}  // namespace url_shortener::db
