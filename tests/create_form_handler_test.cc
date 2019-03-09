#include "gtest/gtest.h"
#include "handler_manager.h"

class CreateFormHandlerTest : public ::testing::Test {
protected:

    void SetUp() {
        requestNew.method = "GET";
        requestNew.uri = "/meme/new";
        requestNew.http_version_major = 1;
        requestNew.http_version_minor = 1;

        requestUpdate1.method = "GET";
        requestUpdate1.uri = "/meme/new?update=1";
        requestUpdate1.http_version_major = 1;
        requestUpdate1.http_version_minor = 1;

        requestUpdate2.method = "GET";
        requestUpdate2.uri = "/meme/new?update=5";
        requestUpdate2.http_version_major = 1;
        requestUpdate2.http_version_minor = 1;

        parser.Parse("test.conf", &config);
        manager = new http::server::HandlerManager(config);
        handler = manager->createByUrl("/meme/new");

        responseNew = handler->HandlerRequest(requestNew);
        resStrNew = responseNew->ToString();

        responseUpdate1 = handler->HandlerRequest(requestUpdate1);
        resStrUpdate1 = responseUpdate1->ToString();

        responseUpdate2 = handler->HandlerRequest(requestUpdate2);
        resStrUpdate2 = responseUpdate2->ToString();

    }

    std::unique_ptr<http::server::RequestHandler> handler;
    NginxConfig config;
    NginxConfigParser parser;
    http::server::HandlerManager* manager;
    http::server::request requestNew;
    http::server::request requestUpdate1;
    http::server::request requestUpdate2;
    std::unique_ptr<http::server::Response> responseNew;
    std::unique_ptr<http::server::Response> responseUpdate1;
    std::unique_ptr<http::server::Response> responseUpdate2;
    std::string resStrNew;
    std::string resStrUpdate1;
    std::string resStrUpdate2;
};

TEST_F(CreateFormHandlerTest, CreateNewResponseOK) {
    EXPECT_EQ(resStrNew.substr(0, 15), "HTTP/1.1 200 OK");
}

TEST_F(CreateFormHandlerTest, CreateNewImageName) {
    EXPECT_TRUE(resStrNew.find("pikachu.jpg") != resStrNew.npos);
    EXPECT_TRUE(resStrNew.find("psyduck.jpg") != resStrNew.npos);
    EXPECT_TRUE(resStrNew.find("wings.jpg") != resStrNew.npos);
}

TEST_F(CreateFormHandlerTest, ValidUpdateResponseOK) {
    EXPECT_EQ(resStrUpdate1.substr(0, 15), "HTTP/1.1 200 OK");
}

TEST_F(CreateFormHandlerTest, ValidUpdateTopBottomValue) {
    EXPECT_TRUE(resStrUpdate1.find("id1_test") != resStrUpdate1.npos);
    EXPECT_TRUE(resStrUpdate1.find("Top text") == resStrUpdate1.npos);
    EXPECT_TRUE(resStrUpdate1.find("Bottom text") == resStrUpdate1.npos);
}

TEST_F(CreateFormHandlerTest, ValidUpdateImageName) {
    EXPECT_TRUE(resStrUpdate1.find("pikachu.jpg") != resStrUpdate1.npos);
    EXPECT_TRUE(resStrUpdate1.find("psyduck.jpg") != resStrUpdate1.npos);
    EXPECT_TRUE(resStrUpdate1.find("wings.jpg") != resStrUpdate1.npos);
}

TEST_F(CreateFormHandlerTest, InvalidUpdateResponseNotFound) {
    EXPECT_EQ(resStrUpdate2.substr(0, 22), "HTTP/1.1 404 Not Found");
}

TEST_F(CreateFormHandlerTest, InvalidUpdateResponseContent) {
    EXPECT_TRUE(resStrUpdate2.find("Invalid ID") != resStrUpdate2.npos);
}