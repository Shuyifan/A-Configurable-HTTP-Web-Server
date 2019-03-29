#include <string>
#include "gtest/gtest.h"
#include "request.h"
#include "handler/bad_request_handler.h"

class BadRequestHandlerTest : public ::testing::Test {
  protected:
    http::server::request request_;
    http::server::RequestHandler* requestHandler = new http::server::BadRequestHandler();
};

TEST_F(BadRequestHandlerTest, badRequestHandlerTest) {
    std::unique_ptr<http::server::Response> response_;
    request_.method = "GET";
    request_.uri = "/";
    request_.http_version_major = 1;
    request_.http_version_minor = 1;
    response_ = requestHandler->HandlerRequest(request_);
    std::string res = response_->ToString();
    EXPECT_EQ(res, "HTTP/1.1 400 Bad Request\r\nContent-Type: text/html\r\n\r\n"\
                   "<h1>400: Bad Request</h1>");
}