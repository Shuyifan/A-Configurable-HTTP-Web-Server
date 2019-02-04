#include <string>
#include "gtest/gtest.h"
#include "request.h"
#include "static_handler.h"

class StaticHandlerTest : public ::testing::Test {
  protected:
    http::server::request request_;
    //http::server::request_parser request_parser_;
    http::server::RequestHandler* requestHandler = new http::server::StaticHandler();
};

TEST_F(StaticHandlerTest, staticRequest) {
    
}