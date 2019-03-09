#include <fstream>
#include <boost/filesystem.hpp>
#include "gtest/gtest.h"
#include "handler_manager.h"

class AcceptHandlerTest : public ::testing::Test {
protected:

    void SetUp() {
        request.method = "POST";
        request.uri = "/meme/create";
        request.http_version_major = 1;
        request.http_version_minor = 1;
        parser.Parse("test.conf", &config);
        manager = new http::server::HandlerManager(config);
        handler = manager->createByUrl("/meme/create");
    }

    std::unique_ptr<http::server::RequestHandler> handler;
    NginxConfig config;
    NginxConfigParser parser;
    http::server::HandlerManager* manager;
    http::server::request request;
};

TEST_F(AcceptHandlerTest, ValidInput) {
    request.body = "image=psyduck.jpg&top=top&bottom=bottom";
    std::unique_ptr<http::server::Response> response = handler->HandlerRequest(request);
    std::string resStr = response->ToString();

    EXPECT_EQ(resStr.substr(0, 15), "HTTP/1.1 200 OK");
    
    std::ifstream is("files/userMemesTest/meme1");
    char buf[512];
	std::string expected;
    is >> expected;
	while(is.read(buf, sizeof(buf)).gcount() > 0) {
		expected.append(buf, is.gcount());
	}
    EXPECT_EQ(expected, "id 1;\nimage psyduck.jpg;\ntop top;\nbottom bottom;\n");
    boost::filesystem::remove("files/id");
    boost::filesystem::remove_all("files/userMemesTest");
}

TEST_F(AcceptHandlerTest, EmptyInput1) {
    request.body = "image=psyduck.jpg&bottom=bottom";
    std::unique_ptr<http::server::Response> response = handler->HandlerRequest(request);
    std::string resStr = response->ToString();

    EXPECT_EQ(resStr.substr(0, 22), "HTTP/1.1 404 Not Found");

    request.body = "&top=top&bottom=bottom";
    response = handler->HandlerRequest(request);
    resStr = response->ToString();

    EXPECT_EQ(resStr.substr(0, 22), "HTTP/1.1 404 Not Found");

    request.body = "image=psyduck.jpg&top=top";
    response = handler->HandlerRequest(request);
    resStr = response->ToString();

    EXPECT_EQ(resStr.substr(0, 22), "HTTP/1.1 404 Not Found");
}

TEST_F(AcceptHandlerTest, EmptyInput2) {
    std::unique_ptr<http::server::Response> response = handler->HandlerRequest(request);
    std::string resStr = response->ToString();

    EXPECT_EQ(resStr.substr(0, 22), "HTTP/1.1 404 Not Found");
}

TEST_F(AcceptHandlerTest, Update) {
    request.body = "image=psyduck.jpg&top=top&bottom=bottom&update=1";
    std::unique_ptr<http::server::Response> response = handler->HandlerRequest(request);
    std::string resStr = response->ToString();

    EXPECT_EQ(resStr.substr(0, 15), "HTTP/1.1 200 OK");

    EXPECT_FALSE(boost::filesystem::exists("files/id"));

    std::ifstream is("files/userMemesTest/meme1");
    char buf[512];
	std::string expected;
    is >> expected;
	while(is.read(buf, sizeof(buf)).gcount() > 0) {
		expected.append(buf, is.gcount());
	}
    EXPECT_EQ(expected, "id 1;\nimage psyduck.jpg;\ntop top;\nbottom bottom;\n");
    boost::filesystem::remove_all("files/userMemesTest");
}
