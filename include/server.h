
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class session;

class server {
public:
    server(boost::asio::io_service& io_service, short port);

private:
    void start_accept();

    boost::asio::io_service& io_service_;
    tcp::acceptor acceptor_;
};