#include "handler_manager.h"
namespace http {
namespace server {

HandlerManager::HandlerManager(NginxConfig& config) {
    for(const auto& statement : config.statements_) {
		const std::vector<std::string> tokens = statement->tokens_;
		if(tokens[0] == "root") {
			base_dir = tokens[1];
        } else if(tokens[0] == "handler") {
            std::string url = getLocation(*statement->child_block_);
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
    } else if(name == "echo") {
        handler.reset(http::server::EchoHandler::create(config, root_path));
    } else {
        handler.reset(http::server::DefaultHandler::create(config, root_path));
    }
    return handler;
} 

std::unique_ptr<http::server::RequestHandler> HandlerManager::createByUrl(const std::string& url) {
    return createByName(param[url].handler_name, param[url].config, base_dir);
} 

std::string getLocation(NginxConfig& inner_config) {
    for(const auto& statement : inner_config.statements_) {
		const std::vector<std::string> tokens = statement->tokens_;
		if(tokens[0] == "location") {
			return tokens[1];
        }
	}
    return "";
}

}
}