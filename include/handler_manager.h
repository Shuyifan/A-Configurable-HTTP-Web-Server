#ifndef HANDLER_MANAGER_H
#define HANDLER_MANAGER_H
#include <memory>
#include <string>
#include <map>
#include "request_handler.h"
#include "static_handler.h"
#include "echo_handler.h"
#include "default_handler.h"
#include "config_parser.h"
#include "handler_parameter.h"

namespace http {
namespace server {

class HandlerManager {
public:

	HandlerManager(NginxConfig& config);

	std::unique_ptr<http::server::RequestHandler> createByName(const std::string& name, 
									 						   const NginxConfig& config, 
                                     						   const std::string& root_path);

	std::unique_ptr<http::server::RequestHandler> createByUrl(const std::string& url);

private:
	
	std::string getLocation(NginxConfig& inner_config);
	
	std::map<std::string, http::server::handler_factory_parameter> param;
	
	std::string base_dir;
};

}
}
#endif