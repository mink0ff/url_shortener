#include "delete_short_url.hpp"
#include <userver/formats/json/value_builder.hpp>
#include <userver/logging/log.hpp>
#include "../utils/validation.hpp"
#include "../utils/exceptions.hpp"
#include <userver/components/component_context.hpp>

namespace url_shortener::handlers {

DeleteShortUrl::DeleteShortUrl(const userver::components::ComponentConfig& config,
                               const userver::components::ComponentContext& context)
    : HttpHandlerJsonBase(config, context),
      service_(context.FindComponent<service::ShortenerService>()) {}

userver::formats::json::Value DeleteShortUrl::HandleRequestJsonThrow(const userver::server::http::HttpRequest& request,
                                                                const userver::formats::json::Value& json,
                                                                userver::server::request::RequestContext&) const {
    const std::string short_url = json["short_url"].As<std::string>();
    try {
        auto delete_short_url = service_.DeleteShortUrl(short_url);

        request.SetResponseStatus(userver::server::http::HttpStatus::kOk);
        LOG_INFO() << "Deleted short URL: " << delete_short_url;

        return userver::formats::json::MakeObject("deleted_short_url", delete_short_url);

    } catch (const exceptions::DatabaseError& e) {
        request.SetResponseStatus(userver::server::http::HttpStatus::kInternalServerError);
        LOG_ERROR() << "Database error: " << e.what();

        return userver::formats::json::MakeObject("error", "Database error");
    } catch (const exceptions::ValidationError& e) {
        request.SetResponseStatus(userver::server::http::HttpStatus::kBadRequest);
        LOG_ERROR() << "Validation error: " << e.what();

        return userver::formats::json::MakeObject("error", e.what());
    } catch (const exceptions::NotFoundError& e) {
        request.SetResponseStatus(userver::server::http::HttpStatus::kNotFound);
        LOG_ERROR() << "Short URL not found: " << short_url;
        
        return userver::formats::json::MakeObject("error", e.what());
    }
}


} // namespace url_shortener::handlers