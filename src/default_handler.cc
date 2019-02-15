#include <iostream>
#include <sstream>
#include <string>
#include "default_handler.h"
namespace http {
namespace server {

DefaultHandler::DefaultHandler() {}

DefaultHandler::DefaultHandler(std::map<std::string, 
  							            http::server::handler_parameter>& dir_map)
  : http::server::RequestHandler(dir_map) {}

http::server::RequestHandler* DefaultHandler::create(const NginxConfig& config, 
                                                     const std::string& root_path) {
    http::server::DefaultHandler* handler = new http::server::DefaultHandler();
    return handler;
}

bool DefaultHandler::handleRequest(const request& req, std::string& response) {
    std::stringstream res;
    res << "HTTP/1.1 200 OK\r\n";
    res << "Content-Type: text/html\r\n";
    res << "\r\n";
    res << "<h1>Thanks for visiting</h1>";
    res << "<h2>For static file server, go to</h2>";
    res << "<h3>/static/index.html</h3>";
    res << "<h3>/static/test.jpg</h3>";
    res << "<h3>/static/test.txt</h3>";
    res << "<h3>/static/test.zip</h3>";
    res << "<h2>Or</h2>";
    res << "<h3>/foo/index.html</h3>";
    res << "<h3>/foo/test.jpg</h3>";
    res << "<h3>/foo/test.txt</h3>";
    res << "<h3>/foo/test.zip</h3>";
    res << "<h2>For echo server, go to</h2>";
    res << "<h3>/echo</h3>";
    response = res.str();
    return true;
}

std::unique_ptr<http::server::Response> DefaultHandler::HandlerRequest(const request& request) {
    std::unique_ptr<http::server::Response> response_;
    response_->SetVersion("HTTP/1.1 200\r\n");
    response_->SetStatus(Response::ok);
    response_->AddHeader("Content-Type", "text/html\r\n");
    std::string def;
    def.append("\r\n");
    def.append("<h1>Thanks for visiting</h1>");
    def.append("<h2>For static file server, go to</h2>");
    def.append("<h3>/static/index.html</h3>");
    def.append("<h3>/static/test.jpg</h3>");
    def.append("<h3>/static/test.txt</h3>");
    def.append("<h3>/static/test.zip</h3>");
    def.append("<h2>Or</h2>");
    def.append("<h3>/foo/index.html</h3>");
    def.append("<h3>/foo/test.jpg</h3>");
    def.append("<h3>/foo/test.txt</h3>");
    def.append("<h3>/foo/test.zip</h3>");
    def.append("<h2>For echo server, go to</h2>");
    def.append("<h3>/echo</h3>");
    response_->SetContent(def);
    return response_;
}
}
}