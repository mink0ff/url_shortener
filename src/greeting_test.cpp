#include <greeting.hpp>

#include <userver/utest/utest.hpp>

using url_shortener::UserType;

UTEST(SayHelloTo, Basic) {
    EXPECT_EQ(url_shortener::SayHelloTo("Developer", UserType::kFirstTime), "Hello, Developer!\n");
    EXPECT_EQ(url_shortener::SayHelloTo({}, UserType::kFirstTime), "Hello, unknown user!\n");

    EXPECT_EQ(url_shortener::SayHelloTo("Developer", UserType::kKnown), "Hi again, Developer!\n");
    EXPECT_EQ(url_shortener::SayHelloTo({}, UserType::kKnown), "Hi again, unknown user!\n");
}