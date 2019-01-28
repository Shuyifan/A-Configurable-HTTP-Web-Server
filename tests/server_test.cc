#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "server.h"

class ServerTest : public ::testing::Test {
  protected:
    boost::asio::io_service io_service;
};

TEST_F(ServerTest, invalidPort1) {
    EXPECT_EXIT(server server(io_service, -1), 
                ::testing::ExitedWithCode(1), "Error: Invalid port input");
}

TEST_F(ServerTest, invalidPort2) {
    EXPECT_EXIT(server server(io_service, 65536), 
                ::testing::ExitedWithCode(1), "Error: Invalid port input");
}

TEST_F(ServerTest, invalidPort3) {
    EXPECT_EXIT(server server(io_service, 0), 
                ::testing::ExitedWithCode(1), "Error: Invalid port input");
}