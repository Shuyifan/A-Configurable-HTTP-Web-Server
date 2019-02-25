#include <memory>
#include <string>
#include "gtest/gtest.h"
#include "handler_manager.h"
#include "config_parser.h"
#include "request_handler.h"
#include "echo_handler.h"
#include "static_handler.h"
#include "error_handler.h"
#include "default_handler.h"

class HandlerManagerTest : public ::testing::Test {
protected:
    NginxConfigParser config_parser;
    NginxConfig config;
	NginxConfig emptyConfig;
	std::unique_ptr<http::server::RequestHandler> temp_ptr;
};


TEST_F(HandlerManagerTest, initiazationTest) {
    config_parser.Parse("handler_manager_test/test.conf", &config);
    http::server::HandlerManager handler_manager(config);

}

TEST_F(HandlerManagerTest, createByNameTest) {
    config_parser.Parse("handler_manager_test/test.conf", &config);
    http::server::HandlerManager handler_manager(config);
	
	temp_ptr = handler_manager.createByName("echo", emptyConfig, "");
	EXPECT_EQ(typeid(*temp_ptr), typeid(*(http::server::EchoHandler::create(emptyConfig, ""))));

	temp_ptr = handler_manager.createByName("static", emptyConfig, "");
	EXPECT_EQ(typeid(*temp_ptr), typeid(*(http::server::StaticHandler::create(emptyConfig, ""))));

	temp_ptr = handler_manager.createByName("default", emptyConfig, "");
	EXPECT_EQ(typeid(*temp_ptr), typeid(*(http::server::DefaultHandler::create(emptyConfig, ""))));

	temp_ptr = handler_manager.createByName("status", emptyConfig, "");
	EXPECT_EQ(typeid(*temp_ptr), typeid(*(http::server::StatusHandler::create(emptyConfig, ""))));

	temp_ptr = handler_manager.createByName("error", emptyConfig, "");
	EXPECT_EQ(typeid(*temp_ptr), typeid(*(http::server::ErrorHandler::create(emptyConfig, ""))));

	temp_ptr = handler_manager.createByName("", emptyConfig, "");
	EXPECT_EQ(typeid(*temp_ptr), typeid(*(http::server::ErrorHandler::create(emptyConfig, ""))));

	temp_ptr = handler_manager.createByName("unknown", emptyConfig, "");
	EXPECT_EQ(typeid(*temp_ptr), typeid(*(http::server::ErrorHandler::create(emptyConfig, ""))));
}

TEST_F(HandlerManagerTest, createByUrlTest) {
    config_parser.Parse("handler_manager_test/test.conf", &config);
    http::server::HandlerManager handler_manager(config);
	
	temp_ptr = handler_manager.createByUrl("/echo");
	EXPECT_EQ(typeid(*temp_ptr), typeid(*(http::server::EchoHandler::create(emptyConfig, ""))));

	temp_ptr = handler_manager.createByUrl("/status");
	EXPECT_EQ(typeid(*temp_ptr), typeid(*(http::server::StatusHandler::create(emptyConfig, ""))));

	temp_ptr = handler_manager.createByUrl("/default");
	EXPECT_EQ(typeid(*temp_ptr), typeid(*(http::server::DefaultHandler::create(emptyConfig, ""))));

	temp_ptr = handler_manager.createByUrl("/foo");
	EXPECT_EQ(typeid(*temp_ptr), typeid(*(http::server::StaticHandler::create(emptyConfig, ""))));

	temp_ptr = handler_manager.createByUrl("/static");
	EXPECT_EQ(typeid(*temp_ptr), typeid(*(http::server::StaticHandler::create(emptyConfig, ""))));

	temp_ptr = handler_manager.createByUrl("/foo/echo/index.html");
	EXPECT_EQ(typeid(*temp_ptr), typeid(*(http::server::StaticHandler::create(emptyConfig, ""))));

	temp_ptr = handler_manager.createByUrl("/foo/test.jpg");
	EXPECT_EQ(typeid(*temp_ptr), typeid(*(http::server::StaticHandler::create(emptyConfig, ""))));
}