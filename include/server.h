
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class session;

class server {
public:

    // Construct the server to listen on the specified port
    server(int port);

    // Run the server's io_service loop.
    void run();

private:

    // Start the accept operation.
    void start_accept();
    
    void handle_accept(session *new_session, const boost::system::error_code &error);
    
    // Check whether the port is valid.
    bool is_valid(int port);

    // The io_service used to perform asynchronous operations.
    boost::asio::io_service io_service_;
    
    // Acceptor used to listen for incoming connections.
    tcp::acceptor acceptor_;
};