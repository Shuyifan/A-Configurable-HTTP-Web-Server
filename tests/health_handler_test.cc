#include <string>
#include "gtest/gtest.h"
#include "request.h"
#include "health_handler.h"

class HealthHandlerTest : public ::testing::Test {
  protected:
    http::server::request request_;
    http::server::RequestHandler* handler = new http::server::HealthHandler();
};

TEST_F(HealthHandlerTest, HealthHandlerTest) {
    std::unique_ptr<http::server::Response> response_;
    request_.method = "GET";
    request_.uri = "/";
    request_.http_version_major = 1;
    request_.http_version_minor = 1;
    response_ = handler->HandlerRequest(request_);
    std::string res = response_->ToString();
    EXPECT_EQ(res, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n"\
                   "<h1>OK</h1>");
}