#include "create_short_url.hpp"
#include <userver/formats/json/value_builder.hpp>
#include <userver/logging/log.hpp>
#include "../utils/validation.hpp"
#include "../utils/exceptions.hpp"
#include <userver/components/component_context.hpp>

namespace url_shortener::handlers {

CreateShortUrl::CreateShortUrl(const userver::components::ComponentConfig& config,
                               const userver::components::ComponentContext& context)
    : HttpHandlerJsonBase(config, context),
      service_(context.FindComponent<service::ShortenerService>()) {}

userver::formats::json::Value CreateShortUrl::HandleRequestJsonThrow(const userver::server::http::HttpRequest& request,
                                                                const userver::formats::json::Value& json,
                                                                userver::server::request::RequestContext&) const {
    const std::string original_url = json["original_url"].As<std::string>();
    try {
        if (!url_shortener::utils::IsValidUrl(original_url)) {
            request.SetResponseStatus(userver::server::http::HttpStatus::kBadRequest);
            LOG_ERROR() << "Invalid URL format: " << original_url;

            return userver::formats::json::MakeObject("error", "Invalid URL format");
        }

        auto short_url = service_.CreateShortUrl(original_url);

        request.SetResponseStatus(userver::server::http::HttpStatus::kCreated);
        LOG_INFO() << "Created short URL: " << short_url;

        return userver::formats::json::MakeObject("short_url", short_url);

    } catch (const exceptions::DatabaseError& e) {
        request.SetResponseStatus(userver::server::http::HttpStatus::kInternalServerError);
        LOG_ERROR() << "Database error: " << e.what();

        return userver::formats::json::MakeObject("error", "Database error");
    } catch (const exceptions::ValidationError& e) {
        request.SetResponseStatus(userver::server::http::HttpStatus::kBadRequest);
        LOG_ERROR() << "Validation error: " << e.what();

        return userver::formats::json::MakeObject("error", e.what());
    }
}


} // namespace url_shortener::handlers