#include <iostream>
#include <sstream>
#include <string>
#include "echo_handler.h"
namespace http {
namespace server {

http::server::RequestHandler* EchoHandler::create(const NginxConfig& config, 
                                                  const std::string& root_path) {
    http::server::EchoHandler* handler = new http::server::EchoHandler();
    return handler;
}

std::unique_ptr<http::server::Response> EchoHandler::HandlerRequest(const request& request) {
    std::unique_ptr<http::server::Response> response_ (new http::server::Response);
    response_->SetVersion("1.1");
    response_->SetStatus(http::server::Response::ok);
    response_->AddHeader("Content-Type", "text/plain");
    std::stringstream req;
    req << request.method << " " << request.uri << " ";
    req << "HTTP/" << request.http_version_major << "." << request.http_version_minor << "\r\n";
    for(header hd : request.headers) {
        req << hd.name << ": " << hd.value << "\r\n";
    }
    req << "\r\n";
    response_->SetContent(req.str());
    return response_;
}
}
}