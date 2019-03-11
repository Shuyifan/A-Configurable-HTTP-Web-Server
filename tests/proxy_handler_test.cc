#include "gtest/gtest.h"
#include "handler_manager.h"

class ProxyHandlerTest : public ::testing::Test {
  protected:
    void SetUp()
    {
        parser.Parse("test.conf", &config);
        manager = new http::server::HandlerManager(config);
        handler = manager->createByUrl("/ucla");
    }

    std::unique_ptr<http::server::RequestHandler> handler;
    NginxConfig config;
    NginxConfigParser parser;
    http::server::HandlerManager *manager;
    http::server::request request;
    std::unique_ptr<http::server::Response> response;
    std::string resStr;
};

TEST_F(ProxyHandlerTest, RootPathTest) {
    request.method = "GET";
    request.uri = "/ucla";
    request.http_version_major = 1;
    request.http_version_minor = 1;

    response = handler->HandlerRequest(request);
    resStr = response->ToString();

    EXPECT_EQ(resStr.substr(0, 15), "HTTP/1.1 200 OK");
    EXPECT_TRUE(resStr.find("<title>UCLA</title>") != resStr.npos);
}

TEST_F(ProxyHandlerTest, SubPathTest) {
    request.method = "GET";
    request.uri = "/ucla/admission/";
    request.http_version_major = 1;
    request.http_version_minor = 1;

    response = handler->HandlerRequest(request);
    resStr = response->ToString();

    EXPECT_EQ(resStr.substr(0, 15), "HTTP/1.1 200 OK");
    EXPECT_TRUE(resStr.find("<title>Admission | UCLA</title>") != resStr.npos);
}