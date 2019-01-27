
#include <boost/bind.hpp>
#include <boost/asio.hpp>

#include "server.h"
#include "session.h"

server::server(boost::asio::io_service& io_service, short port)
    : io_service_(io_service),
      acceptor_(io_service, tcp::endpoint(tcp::v4(), port)) {
    
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