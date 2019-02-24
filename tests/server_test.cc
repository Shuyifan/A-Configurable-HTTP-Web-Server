#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>
#include "gtest/gtest.h"
#include "server.h"
#include "session.h"
#include "config_parser.h"

class ServerTest : public ::testing::Test {
protected:
    NginxConfigParser config_parser;
    NginxConfig config;
};

TEST_F(ServerTest, invalidPort) {
    config_parser.Parse("server_test/invalid_port1.conf", &config);
    EXPECT_EXIT(server server(config), ::testing::ExitedWithCode(1), "Error: Invalid port input");
    config_parser.Parse("server_test/invalid_port2.conf", &config);
    EXPECT_EXIT(server server(config), ::testing::ExitedWithCode(1), "Error: Invalid port input");
}

TEST_F(ServerTest, validConfig) {
    config_parser.Parse("server_test/valid_config.conf", &config);
    server server(config);
    server.start_accept();
}

TEST_F(ServerTest, serverStopTest) {
    config_parser.Parse("server_test/valid_config.conf", &config);
    server server(config);
    server.start_accept();
    boost::thread runThread(boost::bind(&server::run, &server));
    boost::thread::sleep(boost::get_system_time() + boost::posix_time::seconds(2));
    boost::thread stopThread(boost::bind(&server::stop, &server));
    runThread.join();
    stopThread.join();
}