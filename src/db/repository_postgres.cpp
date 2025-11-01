#include "repository_postgres.hpp"
#include <userver/logging/log.hpp>
#include <userver/components/component_context.hpp>
#include <userver/yaml_config/schema.hpp>
#include <userver/yaml_config/merge_schemas.hpp>


namespace url_shortener::db {

RepositoryPostgres::RepositoryPostgres(const userver::components::ComponentConfig& config,
                                       const userver::components::ComponentContext& context)
    : userver::components::ComponentBase(config, context),
      pg_cluster_(context.FindComponent<userver::components::Postgres>("postgres-db-1").GetCluster()) {
    LOG_INFO() << "RepositoryPostgres initialized";
}

std::optional<std::string> RepositoryPostgres::InsertUrl(const std::string& short_url,
                                                         const std::string& original_url) {
    userver::storages::postgres::Transaction transaction =
        pg_cluster_->Begin("transaction_insert_url",
                           userver::storages::postgres::ClusterHostType::kMaster, {});

    auto res = transaction.Execute(
        "INSERT INTO urls (short_url, original_url) VALUES ($1, $2) "
        "ON CONFLICT (short_url) DO NOTHING",
        short_url, original_url);

    if (res.RowsAffected()) {
        transaction.Commit();
        LOG_INFO() << "InsertUrl: inserted short_url: " << short_url;
        return short_url;
    }

    res = transaction.Execute("SELECT short_url FROM urls WHERE short_url = $1", short_url);
    transaction.Rollback();

    auto result = res.AsSingleRow<std::string>();
    if (result != short_url) {
        LOG_ERROR() << "InsertUrl: unexpected error for short_url: " << short_url;
        return std::nullopt;
    }

    LOG_INFO() << "InsertUrl: short_url already exists: " << short_url;
    return result;
}

std::optional<std::string> RepositoryPostgres::GetUrl(const std::string& short_url) {
    const userver::storages::postgres::ResultSet res = pg_cluster_->Execute(
        userver::storages::postgres::ClusterHostType::kSlave,
        "SELECT original_url FROM urls WHERE short_url = $1", short_url);

    if (res.IsEmpty()) {
        LOG_INFO() << "GetUrl: short_url not found: " << short_url;
        return std::nullopt;
    }

    LOG_INFO() << "GetUrl: found original_url for short_url: " << short_url;
    return res.AsSingleRow<std::string>();
}

userver::yaml_config::Schema RepositoryPostgres::GetStaticConfigSchema() {
    return userver::yaml_config::Schema{R"(
    type: object
    description: Configuration for RepositoryPostgres component
    additionalProperties: false
    properties:
    postgres-component:
        type: string
        description: The Postgres component to use
    )"};
}

}  // namespace url_shortener::db
