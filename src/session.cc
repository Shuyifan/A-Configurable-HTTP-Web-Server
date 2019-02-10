#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/log/trivial.hpp>

#include "session.h"
#include "echo_handler.h"
#include "static_handler.h"
#include "default_handler.h"
#include "utils.h"

using boost::asio::ip::tcp;

session::session(boost::asio::io_service& io_service, 
                 std::map<std::string, 
                          http::server::handler_parameter>& dir_map)
    : socket_(io_service), 
    dir_map_(dir_map) {}

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
        http::server::request_parser::result_type result;
        
        result = request_parser_.parse(request_, data_, data_ + bytes_transferred);
        
        if(result == http::server::request_parser::good) {
            BOOST_LOG_TRIVIAL(info) << "Successfully parse request";
            BOOST_LOG_TRIVIAL(info) << data_;

            http::server::RequestHandler* requestHandler = nullptr;

            if(dir_map_[get_upper_dir(request_.uri)].dir == "/files/static") {
                BOOST_LOG_TRIVIAL(info) << "Request static file ";
                BOOST_LOG_TRIVIAL(info) << get_file_name(request_.uri);
                requestHandler = new http::server::StaticHandler(dir_map_);
            } else if(dir_map_[request_.uri].dir == ".") {
                BOOST_LOG_TRIVIAL(info) << "Request for echo";
                requestHandler = new http::server::EchoHandler(dir_map_);
            } else {
                requestHandler = new http::server::DefaultHandler(dir_map_);
            }

            requestHandler->handleRequest(request_, response);

            boost::asio::async_write(socket_, 
                                     boost::asio::buffer(response),
                                     boost::bind(&session::handle_write, 
                                                 this,
                                                 boost::asio::placeholders::error));
        } else if (result == http::server::request_parser::indeterminate) {
            start();
        } else {
            request_.clear();
            BOOST_LOG_TRIVIAL(warning) << "Invalid request";
            BOOST_LOG_TRIVIAL(info) << data_;
        }
    } else if(error.value() != 2) {
        // error: 2 is just end of file, meaning the request is complete
        BOOST_LOG_TRIVIAL(error) << "Error: " << error.value() << ", " << error.message().c_str();
        request_.clear();
        delete this;
    }
}

void session::handle_write(const boost::system::error_code &error) {
    request_.clear();
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
