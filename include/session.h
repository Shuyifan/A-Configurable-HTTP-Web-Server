
#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class session {
public:
    session(boost::asio::io_service& io_service);

    tcp::socket& socket();
    boost::system::error_code start();

private:
    std::string get_response();
    std::string find_content();

    tcp::socket socket_;
    enum { max_length = 1024 };
    char data_[max_length];
};