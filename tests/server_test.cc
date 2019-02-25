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
    boost::thread::sleep(boost::get_system_time() + boost::posix_time::seconds(1));
    boost::thread stopThread(boost::bind(&server::stop, &server));
    runThread.join();
    stopThread.join();
}

TEST_F(ServerTest, connectionTest) {
    config_parser.Parse("server_test/valid_config.conf", &config);
    server server(config);
    server.start_accept();
    boost::thread runThread(boost::bind(&server::run, &server));
    boost::thread::sleep(boost::get_system_time() + boost::posix_time::seconds(1));
    
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

    socket.read_some(boost::asio::buffer(respond));
    std::string s = respond;
    EXPECT_EQ(s, "HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\n\r\n"\
                 "<h1>404: Page Not Found</h1>");

    boost::thread stopThread(boost::bind(&server::stop, &server));
    runThread.join();
    stopThread.join();
}

TEST_F(ServerTest, multiThreadTest) {
    config_parser.Parse("server_test/valid_config.conf", &config);
    server server(config);
    server.start_accept();
    boost::thread runThread(boost::bind(&server::run, &server));
    boost::thread::sleep(boost::get_system_time() + boost::posix_time::seconds(1));
    
    boost::asio::io_service io_service1;
    boost::asio::ip::tcp::resolver::query query1("localhost", "8888");
    boost::asio::ip::tcp::resolver resolver1(io_service1);
    boost::asio::ip::tcp::resolver::iterator destination1 = resolver1.resolve(query1);
    boost::asio::ip::tcp::resolver::iterator end1;
    boost::asio::ip::tcp::endpoint endpoint1;
    while (destination1 != end1) {
       endpoint1 = *destination1++;
    }
    boost::asio::ip::tcp::socket socket1(io_service1);
    socket1.connect(endpoint1);

    boost::asio::io_service io_service2;
    boost::asio::ip::tcp::resolver::query query2("localhost", "8888");
    boost::asio::ip::tcp::resolver resolver2(io_service2);
    boost::asio::ip::tcp::resolver::iterator destination2 = resolver2.resolve(query2);
    boost::asio::ip::tcp::resolver::iterator end2;
    boost::asio::ip::tcp::endpoint endpoint2;
    while (destination2 != end2) {
       endpoint2 = *destination2++;
    }
    boost::asio::ip::tcp::socket socket2(io_service2);
    socket2.connect(endpoint2);
    
    std::stringstream request;
    
    request << "GET / HTTP/1.1\r\n";
    socket1.write_some(boost::asio::buffer(request.str()));
    request.str("");

    request << "GET /static/test.txt HTTP/1.1\r\n\r\n";
    socket2.write_some(boost::asio::buffer(request.str()));
    request.str("");

    socket2.read_some(boost::asio::buffer(respond));
    std::string s = respond;
    EXPECT_EQ(s, "HTTP/1.1 200 OK\r\nContent-Length: 12\r\nContent-type: text/plain\r\n\r\n"\
                 "Hello world!");

    request << "\r\n";
    socket1.write_some(boost::asio::buffer(request.str()));
    request.str("");

    socket1.read_some(boost::asio::buffer(respond));
    s = respond;
    EXPECT_EQ(s, "HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\n\r\n"\
                 "<h1>404: Page Not Found</h1>");
    
    boost::thread stopThread(boost::bind(&server::stop, &server));
    runThread.join();
    stopThread.join();
}