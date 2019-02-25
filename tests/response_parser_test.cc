#include "gtest/gtest.h"
#include "config_parser.h"

#include "response_parser.h"
#include "response.h"

class ResponseParserTest : public ::testing::Test {
protected:
    http::server::ResponseParser resParser;
    http::server::Response res;
};

TEST_F(ResponseParserTest, SimpleResponse) {
    std::string response = "HTTP/1.1 200 OK\r\n\r\n";
    resParser.parse(res, response);
    ASSERT_FALSE(resParser.is_bad());
    //EXPECT_EQ(response, res.ToString());
    EXPECT_TRUE(resParser.is_good());
}

TEST_F(ResponseParserTest, ValidResponse) {
    std::string response = "HTTP/1.1 200 OK\r\n" \
                           "Connection: Keep-Alive\r\n\r\n";
    resParser.parse(res, response);
    EXPECT_TRUE(resParser.is_good());
    //EXPECT_EQ(response, res.ToString());
}

TEST_F(ResponseParserTest, ValidwithBody) {
    std::string response = "HTTP/1.1 200 OK\r\n" \
                          "Content-Length: 4\r\n" \
                          "Content-Type: text/plain\r\n\r\n" \
                          "test";
  resParser.parse(res, response);
  EXPECT_TRUE(resParser.is_good());
  //EXPECT_EQ(response, res.ToString());
}

TEST_F(ResponseParserTest, InvalidResponse) {
    std::string response = "HTTP/1.1 400\r\n\r\n";
    resParser.parse(res, response);
    EXPECT_TRUE(resParser.is_bad());
}
