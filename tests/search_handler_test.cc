#include <string>
#include "gtest/gtest.h"
#include "handler_manager.h"

class SearchHandlerTest : public ::testing::Test {
protected:

    void SetUp() {
        request.method = "GET";
        request.uri = "/meme/search?q=id1_test";
        request.http_version_major = 1;
        request.http_version_minor = 1;
        parser.Parse("test.conf", &config);
        manager = new http::server::HandlerManager(config);
        handler = manager->createByUrl("/meme/search");
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

TEST_F(SearchHandlerTest, ResponseOK) {
    EXPECT_EQ(resStr.substr(0, 15), "HTTP/1.1 200 OK");
}

TEST_F(SearchHandlerTest, responseTest) {
    std::stringstream ss;
    ss << "";
    // ss << "<th scope=\"row\"><a href=\"view?id=1\">1</a></th>";
    // ss << "<td>wings.jpg</td>";
    // ss << "<td>id1_test</td>";
    // ss << "<td>id1_test</td>";
    EXPECT_TRUE(resStr.find(ss.str()) != resStr.npos);
}
