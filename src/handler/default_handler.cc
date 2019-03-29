#include <iostream>
#include <sstream>
#include <string>
#include "handler/default_handler.h"
namespace http {
namespace server {

http::server::RequestHandler* DefaultHandler::create(const NginxConfig& config, 
                                                     const std::string& root_path) {
    http::server::DefaultHandler* handler = new http::server::DefaultHandler();
    return handler;
}

std::unique_ptr<http::server::Response> DefaultHandler::HandlerRequest(const request& request) {
    std::unique_ptr<http::server::Response> response_ (new http::server::Response);
    response_->SetVersion("1.1");
    response_->SetStatus(http::server::Response::ok);
    response_->AddHeader("Content-Type", "text/html");
    std::stringstream res;
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
    response_->SetContent(res.str());
    return response_;
}
}
}