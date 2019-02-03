#include <string>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class session;

class server {
public:

    // Construct the server to listen on the specified port
    server(int port, std::string base_dir);

    // Run the server's io_service loop.
    void run();

private:
    // Handle the signal to server;
    void handler(const boost::system::error_code& error, int signal_number);

    // Start the accept operation.
    void start_accept();
    
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
};