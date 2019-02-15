#ifndef HANDLER_PARAMETER_H
#define HANDLER_PARAMETER_H
#include <string>
#include "config_parser.h"

namespace http {
namespace server {

struct handler_parameter
{
	std::string dir;
	std::string log;
};

struct handler_factory_parameter {
	std::string handler_name;
	NginxConfig config;
};

} // namespace server
} // namespace http
#endif