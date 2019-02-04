#include <string>
#include <boost/asio.hpp>
#include "config_parser.h"

using boost::asio::ip::tcp;

class session;

class server {
public:

    // Construct the server to listen on the specified port
    server(std::string base_dir, NginxConfig config);

    // Start the first accept operation.
    void start_accept();

    // Run the server's io_service loop.
    void run();

private:
    // Handle the signal to server;
    void handler(const boost::system::error_code& error, int signal_number);
    
    // Do the accept operation.
    void handle_accept(session *new_session, const boost::system::error_code &error);
    
    void initLogging();

    // Check whether the port is valid.
    bool is_valid(int port);

    // The io_service used to perform asynchronous operations.
    boost::asio::io_service io_service_;
    
    // Acceptor used to listen for incoming connections.
    tcp::acceptor acceptor_;

    // The base directory to serve static file
    std::string base_dir_;

    // The port where the server is going to listen to
    int port_;
};