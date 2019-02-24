
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include <memory>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/log/trivial.hpp>

#include <boost/log/trivial.hpp>
#include <boost/log/sinks.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>

#include "request.h"
#include "response.h"
#include "server.h"
#include "session.h"
#include "echo_handler.h"
#include "static_handler.h"
#include "default_handler.h"
#include "handler_manager.h"
#include "request_parser.h"
#include "handler_parameter.h"
#include "handler_manager.h"




using ::testing::_;
using ::testing::AtLeast;
using ::testing::Test;
using boost::asio::ip::tcp;
using ::testing::Return;
/*
#ifdef GTEST
#define private public
#define protected public
*/
class MockSession:public session{
public:
    MockSession(boost::asio::io_service& io_service,http::server::HandlerManager& handlerManager)
    : session(io_service,handlerManager) {}
    //MOCK_METHOD0(socket,boost::asio::ip::tcp::socket& ());
    MOCK_METHOD0(start,void());
    
    
};
TEST(ServerGTest,TestOne){
    NginxConfigParser config_parser;
    NginxConfig config;
    config_parser.Parse("server_test/valid_config.conf", &config);
    boost::asio::io_service io_service;

    http::server::HandlerManager temp = http::server::HandlerManager(config);
    MockSession m(io_service,temp);
    //EXPECT_CALL(m,start()).Times(AtLeast(1));
 
    server s(config);
    //s.start_accept();
}

//#endif

