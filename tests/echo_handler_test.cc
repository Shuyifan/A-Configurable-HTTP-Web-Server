#include <iostream>
#include <sstream>
#include "gtest/gtest.h"
#include "handler_manager.h"
#include "response.h"

class EchoHandlerTest : public ::testing::Test {
  protected:
    void SetUp() {
        request_.method = "GET";
        request_.uri = "/";
        request_.http_version_major = 1;
        request_.http_version_minor = 1;
        parser.Parse("test.conf", &config);
        manager = new http::server::HandlerManager(config);
        handler = manager->createByUrl("/echo");
        response = handler->HandlerRequest(request_);
        resStr = response->ToString();
    }

    http::server::request request_;
    std::unique_ptr<http::server::RequestHandler> handler;
    NginxConfig config;
    NginxConfigParser parser;
    http::server::HandlerManager* manager;
    std::unique_ptr<http::server::Response> response;
    std::string resStr;
};


TEST_F(EchoHandlerTest, EchoRequest_) {
    EXPECT_EQ(resStr, "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nGET / HTTP/1.1\r\n\r\n");
}



/**
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
}**/
