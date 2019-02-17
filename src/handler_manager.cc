#include "handler_manager.h"
#include <boost/log/trivial.hpp>
#include "utils.h"
namespace http {
namespace server {

HandlerManager::HandlerManager(const NginxConfig& config) {
    for(const auto& statement : config.statements_) {
		const std::vector<std::string> tokens = statement->tokens_;
		if(tokens[0] == "root") {
			base_dir = tokens[1];
        } else if(tokens[0] == "handler") {
            std::string url = getLocation(statement->child_block_);
            http::server::handler_factory_parameter temp;
            temp.config = *statement->child_block_;
            temp.handler_name = tokens[1];
            param[url] = temp;
        }
	}
}

std::unique_ptr<http::server::RequestHandler> HandlerManager::createByName(const std::string& name, 
									 					                   const NginxConfig& config, 
                                     					                   const std::string& root_path) {
    std::unique_ptr<http::server::RequestHandler> handler;
    if(name == "static") {
        handler.reset(http::server::StaticHandler::create(config, root_path));
        BOOST_LOG_TRIVIAL(info) << "Request for static file server";
    } else if(name == "echo") {
        handler.reset(http::server::EchoHandler::create(config, root_path));
        BOOST_LOG_TRIVIAL(info) << "Request for echo server";
    } else {
        handler.reset(http::server::DefaultHandler::create(config, root_path));
    }
    return handler;
} 

std::unique_ptr<http::server::RequestHandler> HandlerManager::createByUrl(const std::string& url) {
    std::string upper_dir = url;
    while(!upper_dir.empty() && param.find(upper_dir) == param.end()) {
        upper_dir = get_upper_dir(upper_dir);
    }
    if(!upper_dir.empty()) {
        return createByName(param[upper_dir].handler_name, param[upper_dir].config, base_dir);
    } else {
        return createByName("default", param.begin()->second.config, base_dir);
    }
} 

std::string HandlerManager::getLocation(std::unique_ptr<NginxConfig>& inner_config) {
    for(const auto& statement : inner_config->statements_) {
		const std::vector<std::string> tokens = statement->tokens_;
		if(tokens[0] == "location") {
			return tokens[1];
        }
	}
    return "";
}

}
}