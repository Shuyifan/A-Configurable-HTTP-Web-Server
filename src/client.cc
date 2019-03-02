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
    BOOST_LOG_TRIVIAL(info) << "Setting up client.  Address:  " << address << "; Port: " << port;
    std::unique_ptr<http::server::Client> client (new Client());
    client->address_ = address;
    client->port_ = port;

    return client;
}

std::unique_ptr<http::server::Response> Client::getResponse(request& request)
{
    BOOST_LOG_TRIVIAL(trace) << "Starting client";
    start();
    BOOST_LOG_TRIVIAL(info) << "Opened connection. Writing request to remote server..";
    bool write_successful = write_request(request.rawString());
    if(write_successful) {
        BOOST_LOG_TRIVIAL(info) << "Sent request successfully. About to read response..";
        std::string response_string = read_response();

        BOOST_LOG_TRIVIAL(info) << "Response string has been received. About to parse into Resposne object..";

        std::unique_ptr<http::server::Response> response (new http::server::Response());
        response_parser_.parse(*response, response_string);

        remote_socket_.close();
        if(response_parser_.is_good())
        {
            BOOST_LOG_TRIVIAL(info) << "Successfully parsed response.";
            BOOST_LOG_TRIVIAL(info) << response_string;
            BOOST_LOG_TRIVIAL(trace) << "Parsed response tostring:\n" << response->ToString();
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

bool Client::splitAddress(const std::string& address, std::string& hostnameOut, std::string& portOut, size_t startPos) {
    // Returns true unless there's an error
    
    // Remove an initial http://
    std::size_t colonDoubleSlashPos = address.find("://", startPos);
    if (colonDoubleSlashPos != std::string::npos ) {
        std::string pre = address.substr(0, colonDoubleSlashPos);
        if (pre != "http") 
            BOOST_LOG_TRIVIAL(warning) << "Address of proxy server begins with '*://' but not 'http://' -- " << pre << ":// -- implicitly pretending it began with http";
        return splitAddress(address.substr(colonDoubleSlashPos+3), hostnameOut, portOut, startPos);
    }
    
    // portOut defaults to 80
    portOut = "80";

    std::size_t colonPos = address.find(':', startPos);
    if (colonPos == std::string::npos) {
        hostnameOut = address;
        return true;
    }
    // exclude the case where the colon precedes a slash
    if (colonPos < address.size()-1 && address[colonPos+1] == '/')
        return splitAddress(address, hostnameOut, portOut, colonPos+1);
    // if the colon is the last character, ignore it
    if (colonPos == address.size()-1)
        return false;

    if (colonPos < address.size()) {
        hostnameOut = address.substr(0, colonPos);
        std::string portTemp = address.substr(colonPos+1); 

        // ensure port out doesn't have any trailing slashes
        while (portTemp.size()>0 && portTemp.back()=='/')
            portTemp.pop_back();

        // make sure port contains only digits
        for ( const char c: portTemp )
            if ( c<'0' || c>'9' ) {
                return false;
            }

        portOut = portTemp;
        return true;
    }

    // shouldn't get to this point
    return false;
}

void Client::start()
{
    // connect to remote server, start reading
    BOOST_LOG_TRIVIAL(trace) << "creating tcp endpoint";
    
    // (endpoint resolving adapted from https://stackoverflow.com/a/31314678/6798063)
    tcp::resolver resolver(io_service_);
    std::string hostname, serverport;
    bool success = splitAddress(address_, hostname, serverport);

    BOOST_LOG_TRIVIAL(trace) << "Attempting to resolve tcp endpoint; Host: " << hostname << "; server port: " << serverport;
    tcp::resolver::query query(hostname, serverport);
    tcp::resolver::iterator iter = resolver.resolve(query);
    tcp::endpoint endp = iter->endpoint();

    BOOST_LOG_TRIVIAL(trace) << "connecting socket to endpoint";
    remote_socket_.connect(endp);
    BOOST_LOG_TRIVIAL(info) << "Opened connection with (host) " << address_ << " on port " << port_;
}

bool Client::write_request(std::string request_string)
{
    BOOST_LOG_TRIVIAL(trace) << "Writing the following request to server:\n" << request_string;

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
