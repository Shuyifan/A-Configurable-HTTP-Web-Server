#include <string>
#include "gtest/gtest.h"
#include "request.h"
#include "handler/error_handler.h"

class ErrorHandlerTest : public ::testing::Test {
  protected:
    http::server::request request_;
    http::server::RequestHandler* requestHandler = new http::server::ErrorHandler();
};

TEST_F(ErrorHandlerTest, errorHandlerTest) {
    std::unique_ptr<http::server::Response> response_;
    request_.method = "GET";
    request_.uri = "/";
    request_.http_version_major = 1;
    request_.http_version_minor = 1;
    response_ = requestHandler->HandlerRequest(request_);
    std::string res = response_->ToString();
    EXPECT_EQ(res, "HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\n\r\n"\
                   "<h1>404: Page Not Found</h1>");
}