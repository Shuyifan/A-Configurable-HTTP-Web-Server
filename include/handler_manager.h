#ifndef HANDLER_MANAGER_H
#define HANDLER_MANAGER_H
#include <memory>
#include <string>
#include <map>
#include "request_handler.h"
#include "config_parser.h"
#include "handler_parameter.h"

namespace http {
namespace server {

class HandlerManager {
public:

	HandlerManager(NginxConfig& config);

	std::unique_ptr<http::server::request> createByName(const std::string& name, 
									 					const NginxConfig& config, 
                                     					const std::string& root_path);

	std::unique_ptr<http::server::request> createByUrl(const std::string& url);

private:
	std::map<std::string, http::server::handler_factory_parameter> param;
	std::string base_dir;
};

}
}
#endif