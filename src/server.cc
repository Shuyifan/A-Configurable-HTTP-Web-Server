
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/log/trivial.hpp>

#include "server.h"
#include "session.h"

const int high_invalid_port = 65536;

server::server(int port)
    : io_service_(),
	acceptor_(io_service_, tcp::endpoint(tcp::v4(), port)) {
    if(!this->is_valid(port)) {
		fprintf(stderr, "Error: Invalid port input");
		exit(1);
    }
	BOOST_LOG_TRIVIAL(info) << "Server starts";
    start_accept();
}

void server::start_accept() {
	session *new_session = new session(io_service_);
    acceptor_.async_accept(new_session->socket(),
                		   boost::bind(&server::handle_accept, 
						   			   this, 
									   new_session, 
									   boost::asio::placeholders::error));
}

void server::run() {
	boost::asio::signal_set signals(io_service_, SIGINT);
	signals.async_wait(boost::bind(&server::handler,
							       this,
								   boost::asio::placeholders::error,
								   SIGINT));
	io_service_.run();
}

void server::handle_accept(session *new_session, const boost::system::error_code &error) {
	if(!acceptor_.is_open()) {
		return;
	}
	if(!error) {
		new_session->start();
	} else {
		printf("Connection failed: %d, %s\n", error.value(), error.message().c_str());
		delete new_session;
	}
	start_accept();
}

void server::handler(const boost::system::error_code& error, int signal_number) {
    BOOST_LOG_TRIVIAL(info) << "Server shut down";
    exit(1);
}

bool server::is_valid(int port) {
	if (port >= high_invalid_port || port <= 0) {
		printf("Invalid port input.\n");
		return false;
	}  
	return true;
}