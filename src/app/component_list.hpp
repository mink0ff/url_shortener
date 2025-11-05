// #pragma once
// #include <userver/components/minimal_server_component_list.hpp>
// #include <userver/server/handlers/tests_control.hpp>


// inline auto BuildComponentList() {
//     return userver::components::MinimalServerComponentList()
//         .Append<userver::server::handlers::Ping>()
//         .Append<userver::components::TestsuiteSupport>()
//         .Append<userver::components::HttpClient>()
//         .Append<userver::server::handlers::TestsControl>()
//         // твои компоненты:
//         .Append<url_shortener::db::RepositoryPostgres>()
//         .Append<url_shortener::service::ShortenerService>()
//         .Append<url_shortener::handlers::CreateShortUrl>();
// }
