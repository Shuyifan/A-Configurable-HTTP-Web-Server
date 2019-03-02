#ifndef CLIENT_H
#define CLIENT_H

#include <boost/asio.hpp>
#include "response.h"
#include "response_parser.h"
#include "request.h"

namespace http {
namespace server {
//class ResponseParser; // forward declare

class Client {

public:
    Client();
    static std::unique_ptr<http::server::Client> create(const std::string& address, const int& port);
    std::unique_ptr<http::server::Response> getResponse(request& request);

protected:
    std::string address_;
    int port_;

    boost::asio::io_service io_service_;
    boost::asio::ip::tcp::socket remote_socket_;
    enum { max_length = 1024 };
    char response_data_[max_length];
    http::server::ResponseParser response_parser_;

    void start();
    bool write_request(std::string request_string);
    std::string read_response();

    static bool splitAddress(const std::string& address, std::string& hostnameOut, std::string& portOut, size_t startPos=0);

}; // class client
} // namespace server
} // namespace http

#endif
