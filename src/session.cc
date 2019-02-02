#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/log/trivial.hpp>

#include "session.h"
#include "request_parser.h"
#include "echo_handler.h"

using boost::asio::ip::tcp;

session::session(boost::asio::io_service& io_service)
    : socket_(io_service) {}

tcp::socket& session::socket() {
    return socket_;
}

void session::start() {
    socket_.async_read_some(boost::asio::buffer(data_, max_length),
                        	boost::bind(&session::handle_read,
                        				this,
                        				boost::asio::placeholders::error,
                        				boost::asio::placeholders::bytes_transferred));

}

void session::handle_read(const boost::system::error_code &error,
                          size_t bytes_transferred) {
    if(!error) {
        std::string response;
        bool success = parseRequest(response, bytes_transferred);
        boost::asio::async_write(socket_, 
                                 boost::asio::buffer(response),
                                 boost::bind(&session::handle_write, 
                                             this,
                                             boost::asio::placeholders::error));
    } else if(error.value() != 2) {
        // error: 2 is just end of file, meaning the request is complete
        printf("Error: %d, %s\n", error.value(), error.message().c_str());
        delete this;
    }
}

std::string session::get_response() {
    std::stringstream res;
    res << "HTTP/1.1 200 OK\r\n";
    res << "Content-Type: text/html\r\n";
    res << "Connection: close\r\n\r\n";
    res << data_;
    return res.str();
}

bool session::parseRequest(std::string& response, const size_t bytes_transferred) {
    http::server::request_parser reqParser;

    http::server::request_parser::result_type result;
    result = reqParser.parse(request_, data_, data_ + bytes_transferred);

    if(result == http::server::request_parser::good) {
        BOOST_LOG_TRIVIAL(info) << "Successfully parse request";
        BOOST_LOG_TRIVIAL(info) << data_;
        // TODO: add static file handler and new either
        // EchoHandler or FileHandler based on request url
        http::server::RequestHandler* requestHandler = new http::server::EchoHandler();
        requestHandler->handleRequest(req, response);
        return true;
    }
    BOOST_LOG_TRIVIAL(warning) << "Invalid request";
    return false;
}

void session::handle_write(const boost::system::error_code &error) {
    if(!error) {
        boost::system::error_code ignored_ec;
        socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);
        socket_.async_read_some(boost::asio::buffer(data_, max_length),
                                boost::bind(&session::handle_read, this,
                                            boost::asio::placeholders::error,
                                            boost::asio::placeholders::bytes_transferred));
    } else {
        delete this;
    }
}

// std::string session::find_content() {
//     std::string temp = data_;
//     size_t start = temp.find("\r\n\r\n");
//     if(start + 4 != temp.size()) {
//         return temp.substr(start + 4);
//     } else {
//         return "";
//     }
// }