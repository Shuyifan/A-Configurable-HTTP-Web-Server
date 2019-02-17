#include <string>
#include "gtest/gtest.h"
#include "request.h"
#include "echo_handler.h"

class EchoHandlerTest : public ::testing::Test {
  protected:
    http::server::request request_;
    //http::server::request_parser request_parser_;
    http::server::RequestHandler* requestHandler = new http::server::EchoHandler();
};

TEST_F(EchoHandlerTest, NormalRequest) {
    std::string res;
    bool test = requestHandler->handleRequest(request_, res);
    EXPECT_EQ(test, true);
}

TEST_F(EchoHandlerTest, EchoRequest) {
    std::string res;
    //std::string request = "GET / HTTP/1.1\r\n\r\n";
    request_.method = "GET";
    request_.uri = "/";
    request_.http_version_major = 1;
    request_.http_version_minor = 1;
    bool test = requestHandler->handleRequest(request_, res);
    EXPECT_EQ(res, "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nConnection: close\r\n\r\nGET / HTTP/1.1\r\n\r\n");
}

TEST_F(EchoHandlerTest, EchoRequest_) {
    std::unique_ptr<http::server::Response> response_;
    request_.method = "GET";
    request_.uri = "/";
    request_.http_version_major = 1;
    request_.http_version_minor = 1;
    response_ = requestHandler->HandlerRequest(request_);
    std::string res = response_->ToString();
    EXPECT_EQ(res, "HTTP/1.1 200 OK\r\nContent-Length: 18\r\nContent-Type: text/plain\r\nGET / HTTP/1.1\r\n\r\n");
}