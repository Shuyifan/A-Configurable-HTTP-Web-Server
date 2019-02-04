#include <iostream>
#include <sstream>
#include <string>
#include "default_handler.h"
namespace http {
namespace server {

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

}
}