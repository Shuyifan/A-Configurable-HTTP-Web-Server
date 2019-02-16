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
    Construct the server to listen on the specified port
    @param NginxCongif 
    */
    server(const NginxConfig& config);
    
    /**
    Start the first accept operation.
    */
    void start_accept();

    /**
    Run the server's io_service loop and start logging 
    the information about the server
    */
    void run();

private:
    
    /**
    Handle the signal to server and log the error

    @param boost::system::error_code, signal_number
    @return 
    */
    void handler(const boost::system::error_code& error, int signal_number);
    
    /**
    Do the accept operation

    @param boost::system::error_code, signal_number
    @return 
    */
    void handle_accept(session *new_session, const boost::system::error_code &error);
     
    /**
    Prepare the log file and log settings using boost log library
    */
    void initLogging();

    /**
    Check whether the port is valid.

    @param integer port number
    @return bool of is it valid or not
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
};