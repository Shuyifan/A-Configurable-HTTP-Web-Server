
#include <boost/bind.hpp>
#include <boost/asio.hpp>

#include "server.h"
#include "session.h"


const int low_invalid_port = 1023;
const int high_invalid_port = 65536;

server::server(boost::asio::io_service& io_service, int port)
    : io_service_(io_service),
      acceptor_(io_service, tcp::endpoint(tcp::v4(), port)) {
    if(!this->is_valid(port)) {
      fprintf(stderr, "Error: Invalid port input");
      exit(1);
    }
    start_accept();
}

void server::start_accept() {
    session* new_session = new session(io_service_);
    boost::system::error_code error;
    acceptor_.accept(new_session->socket(), error);

    if(error) {
      printf("Connection failed: %d, %s\n",
             error.value(), error.message().c_str());
      delete new_session;
    } else {
      auto err = new_session->start();
    }

    start_accept();

}

  bool server::is_valid(int port) {
    if (port >= high_invalid_port) {
      printf("Invalid port input.\n");
      return false;
    }  
    return true;
  }