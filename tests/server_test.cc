#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "server.h"
#include "session.h"
#include "config_parser.h"

class ServerTest : public ::testing::Test {
protected:
    NginxConfigParser config_parser;
    NginxConfig config;
};

/*
class MockSession : public session {
public:
    MOCK_METHOD0(handle_write,void(const boost::system::error_code));
    MOCK_METHOD1(handle_read,void(const boost::system::error_code,size_t));
    MOCK_METHOD2(start,void());
};

TEST(ServerTest，TryTest){
    MockSession mb;
    server s;
    
    NginxConfigParser config_parser;
    NginxConfig config;
    config_parser.Parse(" ../conf/deploy.conf ", &config);
    server s(config);
    EXPECT_CALL(mb,start()).Times(1)
}*/

TEST_F(ServerTest, invalidPort) {
    config_parser.Parse("server_test/invalid_port1.conf", &config);
    EXPECT_EXIT(server server(config), ::testing::ExitedWithCode(1), "Error: Invalid port input");
    config_parser.Parse("server_test/invalid_port2.conf", &config);
    EXPECT_EXIT(server server(config), ::testing::ExitedWithCode(1), "Error: Invalid port input");
    config_parser.Parse("server_test/invalid_port3.conf", &config);
    EXPECT_EXIT(server server(config), ::testing::ExitedWithCode(1), "Error: Invalid port input");
}

TEST_F(ServerTest, validConfig) {
    config_parser.Parse("server_test/valid_config.conf", &config);
    server server(config);
    server.start_accept();
}