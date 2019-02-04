#include <iostream>
#include <boost/asio.hpp>
#include "request.h"
#include "request_parser.h"

using boost::asio::ip::tcp;

class session {
public:
    session(boost::asio::io_service& io_service, 
            std::map<std::string, std::string>& dir_map);
    std::string get_response();
    //bool parseRequest(std::string& response, const size_t length);

    tcp::socket& socket();
    
    // Start the first asynchronous operation for the connection.
    void start();

private:
    // Perform an asynchronous read operation.
    void handle_write(const boost::system::error_code &error);
    
    // Perform an asynchronous write operation.
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
    std::map<std::string, std::string> dir_map_;
};