#include "gtest/gtest.h"
#include <iostream>
#include "response.h"

class ResponseTest : public ::testing::Test {
protected:
    void SetUp() override {}

    http::server::Response res;
};
//this test test the resString's functionality
TEST_F(ResponseTest, TestToString) {
    res.SetVersion("1.1");
    res.SetStatus(http::server::Response::StatusCode::ok);
    EXPECT_EQ("HTTP/1.1 200 OK\r\n\r\n",res.ToString());
    res.SetStatus(http::server::Response::StatusCode::created);
    EXPECT_EQ("HTTP/1.1 201 Created\r\n\r\n",res.ToString());
    res.SetStatus(http::server::Response::StatusCode::accepted);
    EXPECT_EQ("HTTP/1.1 202 Accepted\r\n\r\n",res.ToString());
    res.SetStatus(http::server::Response::StatusCode::no_content);
    EXPECT_EQ("HTTP/1.1 204 No Content\r\n\r\n",res.ToString());
    res.SetStatus(http::server::Response::StatusCode::multiple_choices);
    EXPECT_EQ("HTTP/1.1 300 Multiple Choices\r\n\r\n",res.ToString());
    res.SetStatus(http::server::Response::StatusCode::moved_permanently);
    EXPECT_EQ("HTTP/1.1 301 Moved Permanently\r\n\r\n",res.ToString());
    res.SetStatus(http::server::Response::StatusCode::moved_temporarily);
    EXPECT_EQ("HTTP/1.1 302 Moved Temporarily\r\n\r\n",res.ToString());
    res.SetStatus(http::server::Response::StatusCode::not_modified);
    EXPECT_EQ("HTTP/1.1 304 Not Modified\r\n\r\n",res.ToString());
    res.SetStatus(http::server::Response::StatusCode::bad_request);
    EXPECT_EQ("HTTP/1.1 400 Bad Request\r\n\r\n",res.ToString());
    res.SetStatus(http::server::Response::StatusCode::unauthorized);
    EXPECT_EQ("HTTP/1.1 401 Unauthorized\r\n\r\n",res.ToString());
    res.SetStatus(http::server::Response::StatusCode::forbidden);
    EXPECT_EQ("HTTP/1.1 403 Forbidden\r\n\r\n",res.ToString());
    res.SetStatus(http::server::Response::StatusCode::not_found);
    EXPECT_EQ("HTTP/1.1 404 Not Found\r\n\r\n",res.ToString());
}
//this is for testing to string
// TEST_F(ResponseTest, Testtostring) {
//     res.SetStatus(http::server::Response::StatusCode::ok);
//     EXPECT_EQ("HTTP/1.1 200 OK\r\nContent-Length: 0\r\n",res.ToString());
//     res.SetStatus(http::server::Response::StatusCode::created);
//     EXPECT_EQ("HTTP/1.1 201 Created\r\nContent-Length: 0\r\n",res.ToString());
   
// }