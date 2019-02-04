#include <string>
#include "gtest/gtest.h"
#include "request.h"
#include "static_handler.h"

class StaticHandlerTest : public ::testing::Test {
  protected:
    http::server::request request_;
    //std::map<std::string, std::string>& dir_map;
    //http::server::RequestHandler* requestHandler = new http::server::StaticHandler(dir_map_);
};

// TEST_F(StaticHandlerTest, NormalUri) {
//     const std::string in = "/files/static/index.html";
//     std::string out;
//     bool test = requestHandler->url_decode(in, out);
//     EXPECT_EQ(test, true);
// }

// TEST_F(StaticHandlerTest, BadUri) {
    
// }
//现在比较好写的测试有util.cc里的get_file_name, get_upper_dir。static_handler.cc里的uri_decoder。server.cc里的server，initLogging。这些