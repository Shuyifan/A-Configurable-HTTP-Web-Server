
#include <iostream>
#include <boost/asio.hpp>
#include "request.h"

using boost::asio::ip::tcp;

class session {
public:
    session(boost::asio::io_service& io_service);
    std::string get_response();
    bool parseRequest(std::string& response, const size_t length);

    tcp::socket& socket();
    void start();

private:
    void handle_write(const boost::system::error_code &error);
    void handle_read(const boost::system::error_code &error,
                          size_t bytes_transferred);
    //std::string find_content();

    // The incoming request
    http::server::request request_;

    tcp::socket socket_;
    enum { max_length = 1024 };
    char data_[max_length];
};