#include <iostream>
#include <sstream>
#include <memory>

#include "bad_request_handler.h"

namespace http {
namespace server {

RequestHandler* BadRequestHandler::create(const NginxConfig& config, 
								  const std::string& root_path) {
    BadRequestHandler* handler = new BadRequestHandler();
    return handler;
}

std::unique_ptr<http::server::Response> BadRequestHandler::HandlerRequest(const request& request) {
    std::unique_ptr<Response> response_ (new Response);
    response_->SetStatus(Response::bad_request);
    response_->SetVersion("1.1");
    response_->AddHeader("Content-Type", "text/html");
    response_->SetContent("<h1>400: Bad Request</h1>");
    return response_;
}

}
}