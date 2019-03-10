#include "gtest/gtest.h"
#include "handler_manager.h"

class ViewMemeHandlerTest : public ::testing::Test {
protected:

    void SetUp() {
        parser.Parse("test.conf", &config);
        manager = new http::server::HandlerManager(config);
        handler = manager->createByUrl("/meme/view");
    }

    std::unique_ptr<http::server::RequestHandler> handler;
    NginxConfig config;
    NginxConfigParser parser;
    http::server::HandlerManager* manager;
    http::server::request request;
};

TEST_F(ViewMemeHandlerTest, NormalRequest) {
    request.method = "GET";
    request.uri = "/meme/view?id=1";
    request.http_version_major = 1;
    request.http_version_minor = 1;
    std::unique_ptr<http::server::Response> response = handler->HandlerRequest(request);
    std::string resStr = response->ToString();

    EXPECT_EQ(resStr.substr(0, 15), "HTTP/1.1 200 OK");
    EXPECT_TRUE(resStr.find("wings.jpg") != resStr.npos);
    EXPECT_TRUE(resStr.find("id1_test") != resStr.npos);
    EXPECT_TRUE(resStr.find("update=1") != resStr.npos);
}

TEST_F(ViewMemeHandlerTest, InvalidId) {
    request.method = "GET";
    request.uri = "/meme/view?id=5";
    request.http_version_major = 1;
    request.http_version_minor = 1;
    std::unique_ptr<http::server::Response> response = handler->HandlerRequest(request);
    std::string resStr = response->ToString();

    EXPECT_EQ(resStr.substr(0, 22), "HTTP/1.1 404 Not Found");

    EXPECT_TRUE(resStr.find("Invalid") != resStr.npos);
}

TEST_F(ViewMemeHandlerTest, EscapeUserInput) {
    request.method = "GET";
    request.uri = "/meme/view?id=4";
    request.http_version_major = 1;
    request.http_version_minor = 1;
    std::unique_ptr<http::server::Response> response = handler->HandlerRequest(request);
    std::string resStr = response->ToString();

    EXPECT_EQ(resStr.substr(0, 15), "HTTP/1.1 200 OK");

    EXPECT_FALSE(resStr.find("<script>") != resStr.npos);
    EXPECT_TRUE(resStr.find("&lt;script&gt;") != resStr.npos);
}