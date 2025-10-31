#pragma once

#include <userver/storages/postgres/cluster.hpp>
#include <optional>
#include <string>

namespace url_shortener::db {

class RepositoryPostgres {
public:
    explicit RepositoryPostgres(userver::storages::postgres::ClusterPtr cluster);

    std::optional<std::string> InsertUrl(const std::string&short_url, const std::string& original_url);

    std::optional<std::string> GetUrl(const std::string& short_url);

private:
    userver::storages::postgres::ClusterPtr pg_cluster_;
};  // class RepositoryPostgres

}  // namespace url_shortener::db