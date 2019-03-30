#include <string>
#include <boost/asio.hpp>
#include <map>
#include <string>
#include "config_parser.h"
#include "handler_parameter.h"
#include "handler_manager.h"

using boost::asio::ip::tcp;

class session;

class server {
public:

    /**
     * Construct the server according a specific config.
     * @param config The input config. 
    */
    server(const NginxConfig& config);
    
    /**
     * Start the first accept operation.
    */
    void start_accept();

    void run();
    
   void stop();

private:
    
    /**
     * Handle the signal to server and log the error.
     * @param error_code The error code if there is any.
     * @param signal_number The signal the sever receieve (e.g. Stop signal by Ctrl + C).
    */
    void handler(const boost::system::error_code& error, int signal_number);
    
    /**
    Do the accept operation
     * @param new_session The session class (a class deals with IO operation) used for the server.
     * @param error The error code if there is any.
    */
    void handle_accept(session *new_session, const boost::system::error_code &error);
    
    void initLogging();

    /**
    Check whether the port is valid.
     * @param  integer port number
     * @return bool of is it valid or not
    */
    bool is_valid(int port);

    // The io_service used to perform asynchronous operations.
    boost::asio::io_service io_service_;
    
    // Acceptor used to listen for incoming connections.
    tcp::acceptor acceptor_;

    // The port where the server is going to listen to
    int port_;

    // The handlerManager
    http::server::HandlerManager handlerManager_;

    // The number of core of the server
    int thread_pool_size_;
};