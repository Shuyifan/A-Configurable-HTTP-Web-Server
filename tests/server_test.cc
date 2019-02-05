#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "server.h"

class ServerTest : public ::testing::Test {   
protected:
    NginxConfig out_config;
    NginxConfigParser parser;
};

TEST_F(ServerTest, invalidPort) {
    parser.Parse("server_test/invalid_port1.conf", &out_config);
    EXPECT_EXIT(server server(out_config), 
                ::testing::ExitedWithCode(1), 
                "Error: Invalid port input");

    parser.Parse("server_test/invalid_port2.conf", &out_config);
    EXPECT_EXIT(server server(out_config), 
                ::testing::ExitedWithCode(1), 
                "Error: Invalid port input");

    parser.Parse("server_test/invalid_port3.conf", &out_config);
    EXPECT_EXIT(server server(out_config), 
                ::testing::ExitedWithCode(1), 
                "Error: Invalid port input");
}

TEST_F(ServerTest, invalidPath) {
    parser.Parse("server_test/invalid_path.conf", &out_config);
    EXPECT_EXIT(server server(out_config), 
                ::testing::ExitedWithCode(1), 
                "Error: Invalid path input");
}

TEST_F(ServerTest, validConfig) {
    parser.Parse("server_test/valid_config.conf", &out_config);
    server server(out_config);
    server.start_accept();
}