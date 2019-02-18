#include <string>
#include "gtest/gtest.h"
#include "request.h"
#include "default_handler.h"

class DefaultHandlerTest : public ::testing::Test {
  protected:
    http::server::request request_;
    http::server::RequestHandler* handler = new http::server::DefaultHandler();
};

TEST_F(DefaultHandlerTest, DefaultHandlerTest) {
    std::unique_ptr<http::server::Response> response_;
    request_.method = "GET";
    request_.uri = "/";
    request_.http_version_major = 1;
    request_.http_version_minor = 1;
    response_ = handler->HandlerRequest(request_);
    std::string res = response_->ToString();
    EXPECT_EQ(res, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n"\
                   "<h1>Thanks for visiting</h1>"\
                   "<h2>For static file server, go to</h2>"\
                   "<h3>/static/index.html</h3>"\
                   "<h3>/static/test.jpg</h3>"\
                   "<h3>/static/test.txt</h3>"\
                   "<h3>/static/test.zip</h3>"\
                   "<h2>Or</h2>"\
                   "<h3>/foo/index.html</h3>"\
                   "<h3>/foo/test.jpg</h3>"\
                   "<h3>/foo/test.txt</h3>"\
                   "<h3>/foo/test.zip</h3>"\
                   "<h2>For echo server, go to</h2>"\
                   "<h3>/echo</h3>");
}