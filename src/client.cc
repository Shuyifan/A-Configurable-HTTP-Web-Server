// Copyright (c) 2003-2018 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// https://www.boost.org/doc/libs/1_69_0/doc/html/boost_asio/example/cpp11/chat/client.cpp
// https://github.com/ArashPartow/proxy/blob/master/tcpproxy_server.cpp

#include <cstdlib>
#include <iostream>
#include <string>

#include <boost/asio.hpp>
#include <boost/log/trivial.hpp>
#include <boost/system/error_code.hpp>

#include "client.h"
#include "response.h"

using boost::asio::ip::tcp;
namespace http {
namespace server {

Client::Client()
    : remote_socket_(io_service_)
{
}

std::unique_ptr<http::server::Client> Client::create(const std::string& address, const int& port)
{
    std::unique_ptr<http::server::Client> client (new Client());
    client->address_ = address;
    client->port_ = port;

    return client;
}

std::unique_ptr<http::server::Response> Client::getResponse(request& request)
{
    start();
    BOOST_LOG_TRIVIAL(info) << "Opened connection. Writing request to remote server..";
    bool write_successful = write_request(request.rawString());
    if(write_successful) {
        BOOST_LOG_TRIVIAL(info) << "Sent request successfully. About to read response..";
        std::string response_string = read_response();

        BOOST_LOG_TRIVIAL(info) << "Response string has been received. About to parse into Resposne object..";

        std::unique_ptr<http::server::Response> response (new http::server::Response());
        
        response_parser_.parse(*response,
                            response_string);

        remote_socket_.close();
        if(response_parser_.is_good())
        {
            BOOST_LOG_TRIVIAL(info) << "Successfully parsed response.";
            BOOST_LOG_TRIVIAL(info) << response_string;
            return response;
        } else {
            BOOST_LOG_TRIVIAL(error) << "Invalid response.";
            BOOST_LOG_TRIVIAL(info) << response_data_;
            return nullptr;
        }
    } else {
        BOOST_LOG_TRIVIAL(error) << "Could not receive response due to error sending request.";
        BOOST_LOG_TRIVIAL(info) << request.rawString();
        return nullptr;
    }
}

void Client::start()
{
    // connect to remote server, start reading
    tcp::endpoint endp = tcp::endpoint(boost::asio::ip::address::from_string(address_),
                                        port_);
    remote_socket_.connect(endp);
    BOOST_LOG_TRIVIAL(info) << "Opened connection with (host) " << address_ << " on port " << port_;
}

bool Client::write_request(std::string request_string)
{
    boost::system::error_code error;
    remote_socket_.send(boost::asio::buffer(request_string),
                        {},
                        error);
    if(error)
    {
        BOOST_LOG_TRIVIAL(error) << "Could not send request to server host " << address_ << " on port " << port_;
        return false;
    }
    BOOST_LOG_TRIVIAL(info) << "Sent modified request to remote host " << address_ << " on port " << port_;
    return true;
}

std::string Client::read_response()
{
    std::string response_string;
    boost::system::error_code error;
    size_t bytes_transferred;
    while(!error)
    {
        bytes_transferred = remote_socket_.receive(boost::asio::buffer(response_data_),
                                                          {},
                                                          error); // flags set to empty
        if(!error || error.value() == 2) // error == 2, reached EOF
        {
            response_string.append(response_data_, response_data_ + bytes_transferred);
        }
    }
    BOOST_LOG_TRIVIAL(info) << "Read response data of size " << response_string.size();
    return response_string;
}

}
}
