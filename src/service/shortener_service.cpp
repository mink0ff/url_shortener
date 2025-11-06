#include "shortener_service.hpp"
#include "../utils/id_generator.hpp"
#include "../utils/validation.hpp"
#include "../utils/exceptions.hpp"
#include <userver/components/component_context.hpp>
#include <userver/logging/log.hpp>
#include <userver/yaml_config/schema.hpp>
#include <userver/yaml_config/merge_schemas.hpp>
#include <userver/components/component_list.hpp>
#include <userver/components/component_base.hpp>

namespace url_shortener::service {

ShortenerService::ShortenerService(
    const userver::components::ComponentConfig& config,
    const userver::components::ComponentContext& context)
    : userver::components::ComponentBase(config, context),
      repository_(context.FindComponent<db::RepositoryPostgres>(config["repository"].As<std::string>())) {
    LOG_INFO() << "ShortenerService initialized";
}

std::string ShortenerService::CreateShortUrl(const std::string& original_url) {
    LOG_INFO() << "CreateShortUrl called with: " << original_url;

    if (!utils::IsValidUrl(original_url)) {
        throw exceptions::ValidationError("Invalid URL format");
    }

    bool exists = repository_.FindUrl(original_url);
    if (exists) {
        throw exceptions::ValidationError("Original URL already exists");
    }

    const std::string short_code = utils::GenerateShortId();

    auto inserted = repository_.InsertUrl(short_code, original_url);
    if (!inserted.has_value()) {
        throw exceptions::DatabaseError("Failed to insert URL into database");
    }

    LOG_INFO() << "Successfully created short URL: " << short_code;
    return *inserted;
}

std::string ShortenerService::GetOriginalUrl(const std::string& short_code) {
    LOG_INFO() << "GetOriginalUrl called with: " << short_code;

    auto result = repository_.GetUrl(short_code);
    if (!result.has_value()) {
        throw exceptions::NotFoundError("Short URL not found");
    }

    return *result;
}

std::string ShortenerService::DeleteShortUrl(const std::string& short_code) {
    LOG_INFO() << "DeleteShortUrl called with: " << short_code;

    bool deleted = repository_.DeleteUrl(short_code);
    if (!deleted) {
        throw exceptions::NotFoundError("Short URL not found for deletion");
    }

    LOG_INFO() << "Successfully deleted short URL: " << short_code;
    return short_code;
}


userver::yaml_config::Schema ShortenerService::GetStaticConfigSchema() {
    return userver::yaml_config::MergeSchemas<userver::components::ComponentBase>(R"(
type: object
description: Configuration for ShortenerService component
additionalProperties: false
properties:
  repository:
    type: string
    description: The repository component to use
)");
}
}  // namespace url_shortener::service
