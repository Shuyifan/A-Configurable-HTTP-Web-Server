#include <iostream>
#include <memory>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/log/trivial.hpp>

#include "session.h"
#include "echo_handler.h"
#include "static_handler.h"
#include "default_handler.h"
#include "handler_manager.h"
#include "response.h"
#include "bad_request_handler.h"
using boost::asio::ip::tcp;

session::session(boost::asio::io_service& io_service, 
                 http::server::HandlerManager& handlerManager)
    : socket_(io_service), 
    handlerManager_(handlerManager) {}

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
            BOOST_LOG_TRIVIAL(info) << request_.uri;
           
            std::unique_ptr<http::server::RequestHandler> requestHandler =
                handlerManager_.createByUrl(request_.uri);

            std::unique_ptr<http::server::Response> resp =
                requestHandler->HandlerRequest(request_);

            http::server::HandlerManager::addCount();
            http::server::HandlerManager::addReqResp(request_.uri, resp->getStatusLine());

            boost::asio::async_write(socket_, 
                                     boost::asio::buffer(resp->ToString()),
                                     boost::bind(&session::handle_write, 
                                                 this,
                                                 boost::asio::placeholders::error));
        } else if (result == http::server::request_parser::indeterminate) {
            start();
        } else {
            http::server::BadRequestHandler* handler  = new http::server::BadRequestHandler();
            std::unique_ptr<http::server::Response> resp = handler->HandlerRequest(request_);
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
