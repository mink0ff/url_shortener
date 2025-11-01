#pragma once
#include <stdexcept>

namespace url_shortener::exceptions {

struct NotFoundError : public std::runtime_error {
    using std::runtime_error::runtime_error;
};

struct ValidationError : public std::runtime_error {
    using std::runtime_error::runtime_error;
};

struct DatabaseError : public std::runtime_error {
    using std::runtime_error::runtime_error;
};

}  // namespace url_shortener::exceptions
