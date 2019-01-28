#include "gtest/gtest.h"
#include "config_parser.h"

#include "request_parser.h"
#include "request.h"

class RequestParserTest : public ::testing::Test {
protected:
    void SetUp() override {}

    http::server::request_parser reqParser;
    http::server::request req;
};

TEST_F(RequestParserTest, SimpleRequest) {
    std::string request = "GET / HTTP/1.1\r\n\r\n";
    EXPECT_EQ(reqParser.parse(req, request.begin(), request.end()),
                http::server::request_parser::result_type::good);
}

TEST_F(RequestParserTest, ValidRequest) {
    std::string request = "GET http://www.google.com HTTP/1.1\r\n" \
                           "User-Agent: Mozilla/4.0\r\nAccept-Language: en-US\r\n" \
                           "Accept-Encoding: gzip, dflate\r\n" \
                           "Connection: Keep-Alive\r\n\r\n";
    EXPECT_EQ(reqParser.parse(req, request.begin(), request.end()),
                http::server::request_parser::result_type::good);
}