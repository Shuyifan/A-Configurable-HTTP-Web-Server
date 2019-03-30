#include <iostream>
#include <boost/asio.hpp>
#include "request.h"
#include "request_parser.h"
#include "handler_parameter.h"
#include "handler_manager.h"

using boost::asio::ip::tcp;

/**
 * A class dealing with all the IO operation.
*/
class session {
public:
    session(boost::asio::io_service& io_service, 
            http::server::HandlerManager& handlerManager);

    tcp::socket& socket();

    void start();

private:

    /**
     * Perform an asynchronous read operation.Log the information at the same time
     * This will call different types of handler according to the path.
     * @param error The error code if there is any.
    */
    void handle_write(const boost::system::error_code &error);
    
    /**
     * Perform an asynchronous write operation.
     * @param error The error code if there is any.
     * @param bytes_transferred The number of bytes needed to be transfered.
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
    enum {max_length = 1024};

    // Buffer for incoming data.
    char data_[max_length];

    // The handlerManger
    http::server::HandlerManager handlerManager_;
};