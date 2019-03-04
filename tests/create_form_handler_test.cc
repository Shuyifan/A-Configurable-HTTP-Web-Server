#include "gtest/gtest.h"
#include "handler_manager.h"

class CreateFormHandlerTest : public ::testing::Test {
protected:

    void SetUp() {
        request.method = "GET";
        request.uri = "/meme/new";
        request.http_version_major = 1;
        request.http_version_minor = 1;
        parser.Parse("test.conf", &config);
        manager = new http::server::HandlerManager(config);
        handler = manager->createByUrl("/meme/new");
        response = handler->HandlerRequest(request);
        resStr = response->ToString();
    }

    std::unique_ptr<http::server::RequestHandler> handler;
    NginxConfig config;
    NginxConfigParser parser;
    http::server::HandlerManager* manager;
    http::server::request request;
    std::unique_ptr<http::server::Response> response;
    std::string resStr;
};

TEST_F(CreateFormHandlerTest, ResponseOK) {
    EXPECT_EQ(resStr.substr(0, 15), "HTTP/1.1 200 OK");
}

TEST_F(CreateFormHandlerTest, ImageName) {
    EXPECT_TRUE(resStr.find("pikachu.jpg") != resStr.npos);
    EXPECT_TRUE(resStr.find("psyduck.jpg") != resStr.npos);
    EXPECT_TRUE(resStr.find("wings.jpg") != resStr.npos);
}