#include "gtest/gtest.h"
#include <iostream>
#include "response.h"

class ResponseTest : public ::testing::Test {
protected:
    void SetUp() override {}

    http::server::Response res;
};

TEST_F(ResponseTest, TestGetVersion) {
    http::server::Response res("default");
    std::string ver = res.GetVersion();
    EXPECT_EQ(ver, "1.1");
}

TEST_F(ResponseTest, TestSetVersion) {
    http::server::Response res("default");
    res.SetVersion("1.2");
    std::string ver = res.GetVersion();
    EXPECT_EQ(ver, "1.2");
}

TEST_F(ResponseTest, TestGetStatusCode) {
    http::server::Response res("default");
    http::server::Response::StatusCode code = res.getStatusCode(200);
    EXPECT_EQ(code, http::server::Response::StatusCode::ok);
    code = res.getStatusCode(201);
    EXPECT_EQ(code, http::server::Response::StatusCode::created);
    code = res.getStatusCode(202);
    EXPECT_EQ(code, http::server::Response::StatusCode::accepted);
    code = res.getStatusCode(204);
    EXPECT_EQ(code, http::server::Response::StatusCode::no_content);
    code = res.getStatusCode(300);
    EXPECT_EQ(code, http::server::Response::StatusCode::multiple_choices);
    code = res.getStatusCode(301);
    EXPECT_EQ(code, http::server::Response::StatusCode::moved_permanently);
    code = res.getStatusCode(302);
    EXPECT_EQ(code, http::server::Response::StatusCode::moved_temporarily);
    code = res.getStatusCode(304);
    EXPECT_EQ(code, http::server::Response::StatusCode::not_modified);
    code = res.getStatusCode(400);
    EXPECT_EQ(code, http::server::Response::StatusCode::bad_request);
    code = res.getStatusCode(401);
    EXPECT_EQ(code, http::server::Response::StatusCode::unauthorized);
    code = res.getStatusCode(403);
    EXPECT_EQ(code, http::server::Response::StatusCode::forbidden);
    code = res.getStatusCode(404);
    EXPECT_EQ(code, http::server::Response::StatusCode::not_found);
    code = res.getStatusCode(500);
    EXPECT_EQ(code, http::server::Response::StatusCode::internal_server_error);
    code = res.getStatusCode(501);
    EXPECT_EQ(code, http::server::Response::StatusCode::not_implemented);
    code = res.getStatusCode(502);
    EXPECT_EQ(code, http::server::Response::StatusCode::bad_gateway);
    code = res.getStatusCode(503);
    EXPECT_EQ(code, http::server::Response::StatusCode::service_unavailable);
    code = res.getStatusCode(0);
    EXPECT_EQ(code, http::server::Response::StatusCode::bad_request);
}

TEST_F(ResponseTest, TestgetStatus) {
    http::server::Response res("default");
    res.SetStatus(http::server::Response::StatusCode::ok);
    http::server::Response::StatusCode code = res.getStatus();
    EXPECT_EQ(code, http::server::Response::StatusCode::ok);
}

TEST_F(ResponseTest, TestSetGetStatus) {
    http::server::Response res("default");
    res.SetStatus(http::server::Response::StatusCode::ok);
    http::server::Response::StatusCode code = res.GetStatus();
    EXPECT_EQ(code, http::server::Response::StatusCode::ok);
}

TEST_F(ResponseTest, TestSetGetContent) {
    http::server::Response res("default");
    res.SetContent("hello");
    std::string content = res.getBody();
    EXPECT_EQ(content, "hello");
}

TEST_F(ResponseTest, TestHeaderExists) {
    http::server::Response res("default");
    bool exist = res.headerExists("Content-type");
    EXPECT_EQ(exist, true);
}

TEST_F(ResponseTest, TestgetHeader) {
    http::server::Response res("default");
    std::string header = res.getHeader("Content-type");
    EXPECT_EQ(header, "text/plain");
}


TEST_F(ResponseTest, TestgetStatusLine) {
    http::server::Response res("default");
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