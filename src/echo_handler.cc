#include <iostream>
#include <sstream>
#include <string>
#include "echo_handler.h"
namespace http {
namespace server {

bool EchoHandler::handleRequest(const request& req, std::string& response) {
    std::stringstream res;
    res << "HTTP/1.1 200 OK\r\n";
    res << "Content-Type: text/html\r\n";
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

}
}