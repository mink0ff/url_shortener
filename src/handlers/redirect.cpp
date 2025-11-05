#include "redirect.hpp"
#include <userver/formats/json/value_builder.hpp>
#include <userver/logging/log.hpp>
#include "../utils/validation.hpp"
#include "../utils/exceptions.hpp"
#include <userver/components/component_context.hpp>
#include <userver/server/handlers/exceptions.hpp>

namespace url_shortener::handlers {

Redirect::Redirect(const userver::components::ComponentConfig& config,
                   const userver::components::ComponentContext& context)
    : HttpHandlerBase(config, context),
      service_(context.FindComponent<service::ShortenerService>()) {}

std::string Redirect::HandleRequestThrow(const userver::server::http::HttpRequest& request,
                                         userver::server::request::RequestContext&) const {
    const auto& short_url = request.GetPathArg("short_url");
    
    // if (!url_shortener::utils::IsValidUrl(short_url)) {
    //         request.SetResponseStatus(userver::server::http::HttpStatus::kBadRequest);
    //         auto& response = request.GetHttpResponse();
    //         response.SetStatus(userver::server::http::HttpStatus::kBadRequest);

    //         LOG_ERROR() << "Invalid URL format: " << short_url;

    //         return R"({"error": "Invalid URL format"})";
    // }
    try {
        auto original_url = service_.GetOriginalUrl(short_url);

        request.SetResponseStatus(userver::server::http::HttpStatus::kFound); 
        request.GetHttpResponse().SetHeader(
                std::string_view("Location"),
                original_url
            );

        LOG_INFO() << "Redirecting to original URL: " << original_url;

        return "";
    } catch (const exceptions::DatabaseError& e) {
        request.SetResponseStatus(userver::server::http::HttpStatus::kInternalServerError);
        LOG_ERROR() << "Database error: " << e.what();
        return R"({"error": "Database error"})";
    } catch (const exceptions::NotFoundError&) {
        request.SetResponseStatus(userver::server::http::HttpStatus::kNotFound);
        LOG_ERROR() << "Short URL not found: " << short_url;
        return R"({"error": "Short URL not found"})";
    }
}


} // namespace url_shortener::handlers