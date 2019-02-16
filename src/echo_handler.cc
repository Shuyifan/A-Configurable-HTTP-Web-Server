#include <iostream>
#include <sstream>
#include <string>
#include "echo_handler.h"
namespace http {
namespace server {

EchoHandler::EchoHandler() {}

EchoHandler::EchoHandler(std::map<std::string, 
  							      http::server::handler_parameter>& dir_map)
  : http::server::RequestHandler(dir_map) {}

http::server::RequestHandler* EchoHandler::create(const NginxConfig& config, 
                                                  const std::string& root_path) {
    http::server::EchoHandler* handler = new http::server::EchoHandler();
    return handler;
}

bool EchoHandler::handleRequest(const request& req, std::string& response) {
    std::stringstream res;
    res << "HTTP/1.1 200 OK\r\n";
    res << "Content-Type: text/plain\r\n";
    res << "Connection: close\r\n\r\n";
    res << req.method << " " << req.uri << " ";
    res << "HTTP/" << req.http_version_major << "." << req.http_version_minor << "\r\n";
    for(header hd : req.headers) {
        res << hd.name << ": " << hd.value << "\r\n";
    }
    res << "\r\n";
    response = res.str();
    return true;
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