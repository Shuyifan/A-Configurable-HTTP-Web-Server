#include <string>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include "gtest/gtest.h"
#include "request.h"
#include "handler/static_handler.h"
#include "config_parser.h"
#include "utils.h"
#include "handler_manager.h"
#include "handler_parameter.h"

using boost::asio::ip::tcp;

class StaticHandlerTest : public ::testing::Test {
  protected:
    void SetUp() {
        request_.method = "GET";
        request_.uri = "/..";
        request_.http_version_major = 1;
        request_.http_version_minor = 1;
        parser.Parse("test.conf", &config);
        manager = new http::server::HandlerManager(config);
        handler = manager->createByUrl("/static");
        response = handler->HandlerRequest(request_);
        resStr = response->ToString();
    }

    http::server::request request_;
    http::server::request request2_;
    std::unique_ptr<http::server::RequestHandler> handler;
    NginxConfig config;
    NginxConfigParser parser;
    http::server::HandlerManager* manager;
    std::unique_ptr<http::server::Response> response;
    std::string resStr;
};

TEST_F(StaticHandlerTest, BadRequest) {
    EXPECT_EQ(resStr, "HTTP/1.1 400 Bad Request\r\n\r\n");
}

// TEST_F(StaticHandlerTest, NotFound) {
//     std::unique_ptr<http::server::Response> response_;
//     request_.method = "GET";
//     request_.uri = "/static/empty.txt/";
//     request_.http_version_major = 1;
//     request_.http_version_minor = 1;
//     response_ = requestHandler->HandlerRequest(request_);
//     std::string res = response_->ToString();
//     EXPECT_EQ(res, "HTTP/1.1 404 Not Found\r\n\r\nFile Not Exist!");
// }

// TEST_F(StaticHandlerTest, NotFound) {
//     request_.method = "GET";
//     request_.uri = "/static/empty.txt/";
//     request_.http_version_major = 1;
//     request_.http_version_minor = 1;
//     EXPECT_EQ(resStr, "HTTP/1.1 404 Not Found\r\n\r\nFile Not Exist!");
// }