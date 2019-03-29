#include <string>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include "gtest/gtest.h"
#include "server.h"
#include "request.h"
#include "handler/static_handler.h"
#include "config_parser.h"
#include "utils.h"
#include "handler_manager.h"
#include "handler_parameter.h"

using boost::asio::ip::tcp;

class StatusHandlerTest : public ::testing::Test {
protected:
   NginxConfigParser config_parser;
   NginxConfig config;
   char respond[1024];
};

TEST_F(StatusHandlerTest, multiRequestTest) {
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

   boost::asio::io_service io_service3;
   boost::asio::ip::tcp::resolver::query query3("localhost", "8888");
   boost::asio::ip::tcp::resolver resolver3(io_service3);
   boost::asio::ip::tcp::resolver::iterator destination3 = resolver3.resolve(query3);
   boost::asio::ip::tcp::resolver::iterator end3;
   boost::asio::ip::tcp::endpoint endpoint3;
   while (destination3 != end3) {
      endpoint3 = *destination3++;
   }
   boost::asio::ip::tcp::socket socket3(io_service3);
   socket3.connect(endpoint3);
   
   std::stringstream request;
   
   request << "GET /static/test.jpg HTTP/1.1\r\n\r\n";
   socket1.write_some(boost::asio::buffer(request.str()));
   request.str("");

   socket1.read_some(boost::asio::buffer(respond));
   std::string s = respond;

   request << "GET /static/test.txt HTTP/1.1\r\n\r\n";
   socket2.write_some(boost::asio::buffer(request.str()));
   request.str("");

   socket2.read_some(boost::asio::buffer(respond));
   s = respond;

   request << "GET /status HTTP/1.1\r\n\r\n";
   socket3.write_some(boost::asio::buffer(request.str()));
   request.str("");

   socket3.read_some(boost::asio::buffer(respond));
   s = respond;
   std::cout<<s << std::endl;
   EXPECT_TRUE(s.find("Number of requests the server receives: </h3>2<h3>") != std::string::npos);
   EXPECT_TRUE(s.find("<tr><tr><td>/static/test.txt</td><td>HTTP/1.1 200 OK") != std::string::npos);
   EXPECT_TRUE(s.find("</tr><tr><td>/static/test.jpg</td><td>HTTP/1.1 200 OK") != std::string::npos);

   boost::thread stopThread(boost::bind(&server::stop, &server));
   runThread.join();
   stopThread.join();
}