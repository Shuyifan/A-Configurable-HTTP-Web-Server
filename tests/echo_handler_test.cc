#include <iostream>
#include <sstream>
#include <string>
#include "gmock/gmock.h"
#include "gtest/gtest.h"
//#include "request.h"
#include "echo_handler.h"

class EchoHandlerTest : public ::testing::Test {
  protected:
    http::server::request request_;
    //http::server::request_parser request_parser_;
    http::server::RequestHandler* requestHandler = new http::server::EchoHandler();
};

TEST_F(EchoHandlerTest, NormalRequest) {
    std::string res;
    //std::string request = "GET / HTTP/1.1\r\n\r\n";
    bool test = requestHandler->handleRequest(request_, res);
    EXPECT_EQ(test, true);
}

