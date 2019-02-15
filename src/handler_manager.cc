#include "handler_manager.h"
namespace http {
namespace server {

HandlerManager::HandlerManager(NginxConfig& config) {

}

std::unique_ptr<http::server::request> HandlerManager::createByName(const std::string& name, 
									 					            const NginxConfig& config, 
                                     					            const std::string& root_path) {
    return nullptr;
} 

std::unique_ptr<http::server::request> HandlerManager::createByUrl(const std::string& url) {
    return nullptr;
} 

}
}