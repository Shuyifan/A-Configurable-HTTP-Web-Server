#include <iostream>
#include <sstream>
#include <memory>

#include "error_handler.h"

namespace http {
namespace server {

RequestHandler* ErrorHandler::create(const NginxConfig& config, 
								  const std::string& root_path) {
    ErrorHandler* handler = new ErrorHandler();
    return handler;
}

bool ErrorHandler::handleRequest(const request& req, std::string& response) {
    return true;
}

std::unique_ptr<http::server::Response> ErrorHandler::HandlerRequest(const request& request) {
    std::unique_ptr<Response> response_ (new Response);
    response_->SetStatus(Response::not_found);
    response_->SetVersion("1.1");
    response_->AddHeader("Content-type", "text/html");
    response_->SetContent("<h1>404: Page Not Found</h1>");
    return response_;
}

}
}