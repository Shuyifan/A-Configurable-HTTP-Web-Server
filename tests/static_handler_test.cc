#include <string>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include "gtest/gtest.h"
#include "request.h"
#include "static_handler.h"
#include "config_parser.h"
#include "utils.h"
#include "handler_manager.h"
#include "handler_parameter.h"

using boost::asio::ip::tcp;

class StaticHandlerTest : public ::testing::Test {
  protected:
    http::server::request request_;
    // NginxConfig config;
    // http::server::HandlerManager hm(config);
    // http::server::RequestHandler* requestHandler = hm->createByName("static", hm.param["/static"].config, hm.base_dir);
    http::server::RequestHandler* requestHandler = new http::server::StaticHandler();
    //FIXME: needs to be changed
};

//FIXME: should run successfully after change

// TEST_F(StaticHandlerTest, NormalUri) {
//     std::unique_ptr<http::server::Response> response_;
//     request_.method = "GET";
//     request_.uri = "/static/test.txt";
//     request_.http_version_major = 1;
//     request_.http_version_minor = 1;
//     response_ = requestHandler->HandlerRequest(request_);
//     std::string res = response_->ToString();
//     EXPECT_EQ(res, "HTTP/1.1 200 OK\r\nContent-Length: 12\r\nContent-Type: text/plain\r\n\r\nHello world!");
// }


TEST_F(StaticHandlerTest, BadRequest) {
    std::unique_ptr<http::server::Response> response_;
    request_.method = "GET";
    request_.uri = "/..";
    request_.http_version_major = 1;
    request_.http_version_minor = 1;
    response_ = requestHandler->HandlerRequest(request_);
    std::string res = response_->ToString();
    EXPECT_EQ(res, "HTTP/1.1 400 Bad Request\r\n\r\n");
}

TEST_F(StaticHandlerTest, NotFound) {
    std::unique_ptr<http::server::Response> response_;
    request_.method = "GET";
    request_.uri = "/static/empty.txt/";
    request_.http_version_major = 1;
    request_.http_version_minor = 1;
    response_ = requestHandler->HandlerRequest(request_);
    std::string res = response_->ToString();
    EXPECT_EQ(res, "HTTP/1.1 404 Not Found\r\n\r\nFile Not Exist!");
}

//FIXME: should run successfully after change

// TEST_F(StaticHandlerTest, NoContent) {
//     std::unique_ptr<http::server::Response> response_;
//     request_.method = "GET";
//     request_.uri = "/static/empty.txt";
//     request_.http_version_major = 1;
//     request_.http_version_minor = 1;
//     response_ = requestHandler->HandlerRequest(request_);
//     std::string res = response_->ToString();
//     EXPECT_EQ(res, "HTTP/1.1 204 No Content\r\n\r\n");
// }

// TEST_F(StaticHandlerTest, BadUri) {
    
// }
