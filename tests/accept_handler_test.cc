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

TEST_F(AcceptHandlerTest, MaliciousInput1) {
    request.body = "image=psyduck.jpg&top=<script>&bottom=bottom";
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
    EXPECT_EQ(expected, "id 1;\nimage psyduck.jpg;\ntop &lt;script&gt;;\nbottom bottom;\n");
    boost::filesystem::remove("files/id");
    boost::filesystem::remove_all("files/userMemesTest");
}

TEST_F(AcceptHandlerTest, MaliciousInput2) {
    request.body = "image=psyduck.jpg&top=&&bottom=bottom";
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
    EXPECT_EQ(expected, "id 1;\nimage psyduck.jpg;\ntop &amp;;\nbottom bottom;\n");
    boost::filesystem::remove("files/id");
    boost::filesystem::remove_all("files/userMemesTest");
}
