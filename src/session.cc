#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/algorithm/string.hpp>
#include "session.h"

using boost::asio::ip::tcp;

session::session(boost::asio::io_service& io_service)
    : socket_(io_service) {}

tcp::socket& session::socket() {
    return socket_;
}

void session::start() {
    socket_.async_read_some(boost::asio::buffer(data_, max_length),
                            boost::bind(&session::handle_read, this,
                            boost::asio::placeholders::error,
                            boost::asio::placeholders::bytes_transferred));
}

void session::handle_read(const boost::system::error_code& error,
                          size_t bytes_transferred) {
    if(!error) {
        std::string response_ = get_response();
        boost::asio::async_write(socket_,
                                 boost::asio::buffer(response_),
                                 boost::bind(&session::handle_write, this,
                                 boost::asio::placeholders::error));
    } else {
        delete this;
    }
}

void session::handle_write(const boost::system::error_code& error) {
    if(!error) {
        socket_.async_read_some(boost::asio::buffer(data_, max_length),
                                boost::bind(&session::handle_read, this,
                                boost::asio::placeholders::error,
                                boost::asio::placeholders::bytes_transferred));
    } else {
      delete this;
    }
}

std::string session::get_response() {
    std::stringstream res;
    res << "HTTP/1.1 200 OK\r\n";
    res << "Content-Type: text/html\r\n\r\n";
    res << data_;
    return res.str();
}