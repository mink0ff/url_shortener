#pragma once

#include <userver/server/handlers/http_handler_base.hpp>
#include <userver/formats/json/value.hpp>
#include "../service/shortener_service.hpp"

namespace url_shortener::handlers {

class CreateShortUrl final : public userver::server::handlers::HttpHandlerBase {
public:
    static constexpr std::string_view kName = "create-short-url";

    CreateShortUrl(const userver::components::ComponentConfig& config,
                   const userver::components::ComponentContext& context);

    userver::formats::json::Value HandleRequestJson(const userver::server::http::HttpRequest& request,
                                                    const userver::formats::json::Value& json,
                                                    userver::server::request::RequestContext&) const;

private:
    service::ShortenerService& service_;
};

}  // namespace url_shortener::handlers
