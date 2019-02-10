#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "server.h"
#include "session.h"
/*
class ServerTest : public ::testing::Test {
  protected:
};


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
/**
TEST_F(ServerTest, invalidPort1) {
    EXPECT_EXIT(server server(-1, "/"), 
                ::testing::ExitedWithCode(1), "Error: Invalid port input");
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
}**/