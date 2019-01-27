#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/algorithm/string.hpp>
#include "session.h"
#include "request.h"
#include "request_parser.h"

using boost::asio::ip::tcp;

session::session(boost::asio::io_service& io_service)
    : socket_(io_service) {}

tcp::socket& session::socket() {
    return socket_;
}

boost::system::error_code session::start() {
    boost::system::error_code error;
    
    while(true) {
        const size_t length = 
            socket_.read_some(boost::asio::buffer(data_, max_length), error);

        http::server::request req;
        http::server::request_parser reqParser;
        std::stringstream buf;
        buf << data_;
        std::string input = buf.str();
        http::server::request_parser::result_type result;
        std::string::iterator it;
        result = reqParser.parse(req, input.begin(), input.end());
        
        if(result == http::server::request_parser::good) {
            std::string response_ = get_response();
            boost::asio::write(socket_, boost::asio::buffer(response_));
        }
        if(error) {
            break;
        }
    }
    return error;
}

std::string session::get_response() {
    std::stringstream res;
    res << "HTTP/1.1 200 OK\r\n";
    res << "Content-Type: text/html\r\n\r\n";
    res << find_content();
    return res.str();
}

std::string session::find_content() {
    std::string temp = data_;
    size_t start = temp.find("\r\n\r\n");
    return temp.substr(start + 4);
}