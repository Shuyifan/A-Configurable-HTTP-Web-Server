#include "gtest/gtest.h"
#include <boost/filesystem.hpp>
#include "handler_manager.h"

class DeleteHandlerTest : public ::testing::Test {
protected:

    void SetUp() {
        parser.Parse("test.conf", &config);
        manager = new http::server::HandlerManager(config);
        handler = manager->createByUrl("/meme/delete");
    }

    std::unique_ptr<http::server::RequestHandler> handler;
    NginxConfig config;
    NginxConfigParser parser;
    http::server::HandlerManager* manager;
    http::server::request request;
    std::unique_ptr<http::server::Response> response;
    std::string resStr;
};

TEST_F(DeleteHandlerTest, DeleteTest) {
    std::string testFile = "files/userMemes/meme5";
    std::ofstream os(testFile);
    os.close();

    request.method = "GET";
    request.uri = "/meme/delete?id=5";
    request.http_version_major = 1;
    request.http_version_minor = 1;

    response = handler->HandlerRequest(request);
    resStr = response->ToString();

    EXPECT_EQ(resStr.substr(0, 15), "HTTP/1.1 200 OK");

    EXPECT_FALSE(boost::filesystem::exists("files/userMemes/meme5"));
    EXPECT_TRUE(resStr.find("Delete Success") != resStr.npos);
}

TEST_F(DeleteHandlerTest, InvalidID) {
    request.method = "GET";
    request.uri = "/meme/delete?id=6";
    request.http_version_major = 1;
    request.http_version_minor = 1;

    response = handler->HandlerRequest(request);
    resStr = response->ToString();

    EXPECT_EQ(resStr.substr(0, 22), "HTTP/1.1 404 Not Found");

    EXPECT_TRUE(resStr.find("Invalid ID") != resStr.npos);
}