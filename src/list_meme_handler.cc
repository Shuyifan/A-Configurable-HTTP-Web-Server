#include <iostream>
#include <fstream>
#include <vector>

#include "utils.h"
#include "list_meme_handler.h"

namespace http {
namespace server {
    http::server::RequestHandler* ListMemeHandler::create(const NginxConfig& config,const std::string& root_path) {
        /*TODO:need to fill in the generatl functionality of create*/
        http::server::ListMemeHandler* handler = new http::server::ListMemeHandler();
        return handler;
    }

    std::unique_ptr<http::server::Response> ListMemeHandler::HandlerRequest(const request& request) {
        /*TODO:need to fill in the generatl functionality of hanlderrequest*/
        std::unique_ptr<http::server::Response> response_ (new http::server::Response);
    return response_;
}
    /*
    void CreateFormHandler::setFile(std::string file) {
        file_ = file;
    }*/
} // namespace server
} // namespace http
