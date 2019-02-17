#include "gtest/gtest.h"
#include <iostream>
#include "response.h"

class ResponseTest : public ::testing::Test {
protected:
    void SetUp() override {}

    http::server::Response res;
};
//this test test the resString's functionality
TEST_F(ResponseTest, TestRes) {
    res.SetStatus(http::server::Response::StatusCode::ok);
    EXPECT_EQ("OK",res.GetStringResult(res.GetStatus()));
    res.SetStatus(http::server::Response::StatusCode::created);
    EXPECT_EQ("Created",res.GetStringResult(res.GetStatus()));
    res.SetStatus(http::server::Response::StatusCode::accepted);
    EXPECT_EQ("Accepted",res.GetStringResult(res.GetStatus()));
    res.SetStatus(http::server::Response::StatusCode::no_content);
    EXPECT_EQ("No Content",res.GetStringResult(res.GetStatus()));
    res.SetStatus(http::server::Response::StatusCode::multiple_choices);
    EXPECT_EQ("Multiple Choices",res.GetStringResult(res.GetStatus()));
    res.SetStatus(http::server::Response::StatusCode::moved_permanently);
    EXPECT_EQ("Moved Permanently",res.GetStringResult(res.GetStatus()));
    res.SetStatus(http::server::Response::StatusCode::not_modified);
    EXPECT_EQ( "Not Modified",res.GetStringResult(res.GetStatus()));
    res.SetStatus(http::server::Response::StatusCode::bad_request);
    EXPECT_EQ( "Bad Request",res.GetStringResult(res.GetStatus()));
    res.SetStatus(http::server::Response::StatusCode::unauthorized);
    EXPECT_EQ( "Unauthorized",res.GetStringResult(res.GetStatus()));
    res.SetStatus(http::server::Response::StatusCode::forbidden);
    EXPECT_EQ( "Forbidden",res.GetStringResult(res.GetStatus()));
    res.SetStatus(http::server::Response::StatusCode::not_found);
    EXPECT_EQ( "Not Found",res.GetStringResult(res.GetStatus()));
}
//this is for testing to string
TEST_F(ResponseTest, Testtostring) {
    res.SetStatus(http::server::Response::StatusCode::ok);
    EXPECT_EQ("HTTP/1.1 200 OK\r\nContent-Length: 0\r\n",res.ToString());
    res.SetStatus(http::server::Response::StatusCode::created);
    EXPECT_EQ("HTTP/1.1 201 Created\r\nContent-Length: 0\r\n",res.ToString());
   
}