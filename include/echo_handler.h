#ifndef ECHO_HANDLER_H
#define ECHO_HANDLER_H
#include "request_handler.h"
#include "request.h"
#include "header.h"

namespace http {
namespace server {
class EchoHandler : public RequestHandler {
public:

	EchoHandler();
	/// Construct with a directory containing files to be served.
	EchoHandler(std::map<std::string, 
	    				 http::server::handler_parameter>& dir_map);

	virtual bool handleRequest(const request& req, std::string& response) override;
};
}
}

#endif