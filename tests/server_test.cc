#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>
#include "gtest/gtest.h"
#include "server.h"
#include "session.h"
#include <string>
#include "config_parser.h"

class ServerTest : public ::testing::Test {
protected:
    NginxConfigParser config_parser;
    NginxConfig config;
    char respond[1024];
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

TEST_F(ServerTest, connectionTest) {
    config_parser.Parse("server_test/valid_config.conf", &config);
    server server(config);
    server.start_accept();
    boost::thread runThread(boost::bind(&server::run, &server));
    boost::thread::sleep(boost::get_system_time() + boost::posix_time::seconds(2));
    
    boost::asio::io_service io_service;
    boost::asio::ip::tcp::resolver::query query("localhost", "8888");
    boost::asio::ip::tcp::resolver resolver(io_service);
    boost::asio::ip::tcp::resolver::iterator destination = resolver.resolve(query);
    boost::asio::ip::tcp::resolver::iterator end;
    boost::asio::ip::tcp::endpoint endpoint;
    while ( destination != end ) {
       endpoint = *destination++;
    }
    boost::asio::ip::tcp::socket socket(io_service);
    socket.connect(endpoint);
    
    std::stringstream request;
    request << "GET / HTTP/1.1\r\n\r\n";
    socket.write_some(boost::asio::buffer(request.str()));
    
    boost::thread::sleep(boost::get_system_time() + boost::posix_time::seconds(2));

    socket.read_some(boost::asio::buffer(respond));
    std::string s = respond;
    EXPECT_EQ(s, "HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\n\r\n"\
                 "<h1>404: Page Not Found</h1>");

    boost::thread stopThread(boost::bind(&server::stop, &server));
    runThread.join();
    stopThread.join();
}