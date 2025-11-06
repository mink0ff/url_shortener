#pragma once

#include <userver/server/handlers/http_handler_json_base.hpp>
#include <userver/formats/json/value.hpp>
#include "../service/shortener_service.hpp"

namespace url_shortener::handlers {

class DeleteShortUrl final : public userver::server::handlers::HttpHandlerJsonBase {
public:
    static constexpr std::string_view kName = "delete-short-url";

    DeleteShortUrl(const userver::components::ComponentConfig& config,
                   const userver::components::ComponentContext& context);

    userver::formats::json::Value HandleRequestJsonThrow(
        const userver::server::http::HttpRequest& request,
        const userver::formats::json::Value& json,
        userver::server::request::RequestContext&) const override;

private:
    service::ShortenerService& service_;
};

}  // namespace url_shortener::handlers
