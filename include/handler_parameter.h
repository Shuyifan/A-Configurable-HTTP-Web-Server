#pragma once
#include <string>
#include "config_parser.h"

namespace http {
namespace server {

/**
 * have the current directory and log
*/
struct handler_parameter
{
	std::string dir;
	std::string log;
};

/**
 * Contains the handler name and the config name
*/
struct handler_factory_parameter {
	std::string handler_name;
	NginxConfig config;
};

} // namespace server
} // namespace http
