#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "session.h"

class MockIOService : public boost::asio::io_service {
public:
    
};

class SessionTest : public ::testing::Test {
  protected:
    void SetUp() override {}

    MockIOService io_service;
    session* sess = new session(io_service);
};

TEST_F(SessionTest, GetResponse) {
    std::string response = sess->get_response();
    EXPECT_FALSE(response.empty());
    EXPECT_EQ(response.substr(0, 17), "HTTP/1.1 200 OK\r\n");
}