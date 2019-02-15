#include <iostream>
#include <boost/asio.hpp>
#include "request.h"
#include "request_parser.h"
#include "handler_parameter.h"

using boost::asio::ip::tcp;

class session {
public:
    session(boost::asio::io_service& io_service, 
            std::map<std::string, 
                     http::server::handler_parameter>& dir_map);

    tcp::socket& socket();
    
    /**
    Start the first asynchronous operation for the connection.
    */
    void start();

private:

    /**
    Perform an asynchronous read operation.Log the information at the same time
    This will call different types of handler according to the path
    */
   
    void handle_write(const boost::system::error_code &error);
    
    /**
    Perform an asynchronous write operation.
    @param boost::system::error_code, bytes_transferred
    */
    void handle_read(const boost::system::error_code &error,
                          size_t bytes_transferred);
    
    // The incoming request
    http::server::request request_;

    // The parser for the incoming request.
    http::server::request_parser request_parser_;

    // Socket for the connection.
    tcp::socket socket_;

    // Maximum length for the HTTP request
    enum { max_length = 1024 };

    // Buffer for incoming data.
    char data_[max_length];

    // The map which map the URI to a specific path of the server
    std::map<std::string, http::server::handler_parameter> dir_map_;
};