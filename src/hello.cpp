#include <hello.hpp>

#include <greeting.hpp>

namespace url_shortener {

std::string
Hello::HandleRequestThrow(const userver::server::http::HttpRequest& request, userver::server::request::RequestContext&)
    const {
    return SayHelloTo(request.GetArg("name"), UserType::kFirstTime);
}

}  // namespace url_shortener