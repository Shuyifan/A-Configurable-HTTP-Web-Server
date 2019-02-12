#include <string>
#include "gtest/gtest.h"
#include "request.h"
#include "default_handler.h"

class DefaultHandlerTest : public ::testing::Test {
  protected:
    http::server::request request_;
    http::server::RequestHandler* handler = new http::server::DefaultHandler();
};

TEST_F(DefaultHandlerTest, validRequest) {
    std::string res;
    request_.method = "GET";
    request_.uri = "/";
    request_.http_version_major = 1;
    request_.http_version_minor = 1;
    bool success = handler->handleRequest(request_, res);
    EXPECT_TRUE(success);
}