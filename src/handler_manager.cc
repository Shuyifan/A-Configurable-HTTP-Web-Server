#include "handler_manager.h"
#include <boost/log/trivial.hpp>
#include "utils.h"
namespace http {
namespace server {

int HandlerManager::count = 0;
std::vector<std::pair<std::string, std::string>> HandlerManager::reqResp;

HandlerManager::HandlerManager(const NginxConfig& config) {
    for(const auto& statement : config.statements_) {
		const std::vector<std::string> tokens = statement->tokens_;
		if(tokens[0] == "root") {
			base_dir = tokens[1];
        } else if(tokens[0] == "handler") {
            std::string url = getLocation(statement->child_block_);
            http::server::handler_factory_parameter temp;
            temp.handler_name = tokens[1];
            if(temp.handler_name == "status") {
                temp.config = config;
            } else {
                temp.config = *statement->child_block_;
            }
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
    } else if(name == "error") {
        handler.reset(http::server::ErrorHandler::create(config, root_path));
        BOOST_LOG_TRIVIAL(info) << "Error Handler called";
    } else if(name == "status") {
        handler.reset(http::server::StatusHandler::create(config, root_path));
        BOOST_LOG_TRIVIAL(info) << "Status Handler called";
    } else if(name == "createForm") {
        handler.reset(http::server::CreateFormHandler::create(config, root_path));
        BOOST_LOG_TRIVIAL(info) << "CreateForm Handler called";
    } else if(name == "accept") {
        handler.reset(http::server::AcceptHandler::create(config, root_path));
        BOOST_LOG_TRIVIAL(info) << "Accept Handler called";
    } else if(name == "viewMeme") {
        handler.reset(http::server::ViewMemeHandler::create(config, root_path));
        BOOST_LOG_TRIVIAL(info) << "ViewMeme Handler called";
    } else if(name == "listMeme") {
        handler.reset(http::server::ListMemeHandler::create(config, root_path));
        BOOST_LOG_TRIVIAL(info) << "listMeme Handler called";
    } else if(name == "delete") {
        handler.reset(http::server::DeleteHandler::create(config, root_path));
        BOOST_LOG_TRIVIAL(info) << "Delete Handler called";
    } else if(name == "search") {
        handler.reset(http::server::SearchHandler::create(config, root_path));
        BOOST_LOG_TRIVIAL(info) << "search Handler called";
    } else if(name == "health") {
        handler.reset(http::server::HealthHandler::create(config, root_path));
        BOOST_LOG_TRIVIAL(info) << "listMeme Handler called";
    } else if(name == "default") {
        handler.reset(http::server::DefaultHandler::create(config, root_path));
        BOOST_LOG_TRIVIAL(info) << "Default Handler called";
    } else if(name == "bad") {
        handler.reset(http::server::BadRequestHandler::create(config, root_path));
        BOOST_LOG_TRIVIAL(info) << "Bad Request Handler called";
    } else {
        
        handler.reset(http::server::ErrorHandler::create(config, root_path));
    }

    return handler;
}

std::unique_ptr<http::server::RequestHandler> HandlerManager::createByUrl(const std::string& url) {
    std::string upper_dir = url;
    if(url.find("?") != url.npos) {
        upper_dir = url.substr(0, url.find("?"));
    }
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
