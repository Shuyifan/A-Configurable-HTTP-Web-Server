#include "gtest/gtest.h"
#include <iostream>
#include "request.h"
#include "response.h"
#include "view_meme_handler.h"
#include "utils.h"

class ViewMemeHandlerTest : public ::testing::Test {
    protected:
        http::server::request request_;
        http::server::ViewMemeHandler* requestHandler = new http::server::ViewMemeHandler();
};

TEST_F(ViewMemeHandlerTest, GetID_) {
    const std::string uri = "id=90034?27109";
    int res = requestHandler->getID(uri);
    EXPECT_EQ(res, 90034);
}

// TEST_F(ViewMemeHandlerTest, GenerateHTML_) {
//     int id = 90034;
//     std::string res = requestHandler->generateHTML(id);
    
//     EXPECT_EQ(1, 1);
// }

TEST_F(ViewMemeHandlerTest, ViewMemeRequest_) {
    std::unique_ptr<http::server::Response> response_;
    request_.uri = "id=90034?27109";
    response_ = requestHandler->HandlerRequest(request_);
    std::string version = response_->GetVersion();
    EXPECT_EQ(version, "1.1");
}