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
    response_->SetVersion("HTTP/1.1 200\r\n");
    response_->SetStatus(Response::ok);
    response_->SetMime("text/plain");
    response_->AddHeader("Content-Type", response_->GetMime());
    std::string echo;
    echo.append(request.method);
    echo.append(" ");
    echo.append(request.uri);
    echo.append(" ");
    echo.append("HTTP/");
    echo.append(std::to_string(request.http_version_major));
    echo.append(".");
    echo.append(std::to_string(request.http_version_minor));
    echo.append(" ");
    echo.append("\r\n");
    for(header hd : request.headers) {
        echo.append(hd.name);
        echo.append(": ");
        echo.append(hd.value);
        echo.append("\r\n");
    }
    echo.append("\r\n");
    response_->SetContent(echo);
    return response_;
}
}
}