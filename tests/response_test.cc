#include "gtest/gtest.h"
#include <iostream>
#include "response.h"

class ResponseTest : public ::testing::Test {
protected:
    void SetUp() override {}

    http::server::Response res;
};

TEST_F(ResponseTest, TestSetGetVersion) {
    res.SetVersion("1.2");
    std::string ver = res.GetVersion();
    EXPECT_EQ(ver, "1.2");
}

TEST_F(ResponseTest, TestSetGetStatus) {
    res.SetStatus(http::server::Response::StatusCode::ok);
    http::server::Response::StatusCode code = res.GetStatus();
    EXPECT_EQ(code, http::server::Response::StatusCode::ok);

    res.SetStatus(http::server::Response::StatusCode::created);
    code = res.GetStatus();
    EXPECT_EQ(code, http::server::Response::StatusCode::created);
}

TEST_F(ResponseTest, TestSetGetContent) {
    res.SetContent("hello");
    std::string content = res.getBody();
    EXPECT_EQ(content, "hello");
}

TEST_F(ResponseTest, TestgetStatusLine) {
    res.SetVersion("1.1");
    res.SetStatus(http::server::Response::StatusCode::ok);
    std::string StatusLine = res.getStatusLine();
    EXPECT_EQ(StatusLine, "HTTP/1.1 200 OK\r\n");
}

//this test test the resString's functionality
TEST_F(ResponseTest, TestToString) {
    res.SetVersion("1.1");
    
    res.SetStatus(http::server::Response::StatusCode::ok);
    EXPECT_EQ("HTTP/1.1 200 OK\r\n\r\n", res.ToString());
    
    res.SetStatus(http::server::Response::StatusCode::created);
    EXPECT_EQ("HTTP/1.1 201 Created\r\n\r\n", res.ToString());
    
    res.SetStatus(http::server::Response::StatusCode::accepted);
    EXPECT_EQ("HTTP/1.1 202 Accepted\r\n\r\n", res.ToString());
    
    res.SetStatus(http::server::Response::StatusCode::no_content);
    EXPECT_EQ("HTTP/1.1 204 No Content\r\n\r\n", res.ToString());
    
    res.SetStatus(http::server::Response::StatusCode::multiple_choices);
    EXPECT_EQ("HTTP/1.1 300 Multiple Choices\r\n\r\n", res.ToString());
    
    res.SetStatus(http::server::Response::StatusCode::moved_permanently);
    EXPECT_EQ("HTTP/1.1 301 Moved Permanently\r\n\r\n", res.ToString());
    
    res.SetStatus(http::server::Response::StatusCode::moved_temporarily);
    EXPECT_EQ("HTTP/1.1 302 Moved Temporarily\r\n\r\n", res.ToString());
    
    res.SetStatus(http::server::Response::StatusCode::not_modified);
    EXPECT_EQ("HTTP/1.1 304 Not Modified\r\n\r\n", res.ToString());
    
    res.SetStatus(http::server::Response::StatusCode::bad_request);
    EXPECT_EQ("HTTP/1.1 400 Bad Request\r\n\r\n", res.ToString());
    
    res.SetStatus(http::server::Response::StatusCode::unauthorized);
    EXPECT_EQ("HTTP/1.1 401 Unauthorized\r\n\r\n", res.ToString());
    
    res.SetStatus(http::server::Response::StatusCode::forbidden);
    EXPECT_EQ("HTTP/1.1 403 Forbidden\r\n\r\n", res.ToString());
    
    res.SetStatus(http::server::Response::StatusCode::not_found);
    EXPECT_EQ("HTTP/1.1 404 Not Found\r\n\r\n", res.ToString());

    res.SetStatus(http::server::Response::StatusCode::internal_server_error);    
    EXPECT_EQ("HTTP/1.1 500 Internal Server Error\r\n\r\n", res.ToString());

    res.SetStatus(http::server::Response::StatusCode::not_implemented);
    EXPECT_EQ("HTTP/1.1 501 Not Implemented\r\n\r\n", res.ToString());

    res.SetStatus(http::server::Response::StatusCode::bad_gateway);
    EXPECT_EQ("HTTP/1.1 502 Bad Gateway\r\n\r\n", res.ToString());

    res.SetStatus(http::server::Response::StatusCode::service_unavailable);
    EXPECT_EQ("HTTP/1.1 503 Service Unavailable\r\n\r\n", res.ToString());
}