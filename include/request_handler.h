#ifndef REQUEST_HANDLER_H
#define REQUEST_HANDLER_H
#include <string>
#include <map>
#include <string>
#include <memory>
#include "handler_parameter.h"
#include "request.h"
#include "response.h"

namespace http {
namespace server {
class RequestHandler {
public:
	RequestHandler();
	
	/**
	Construct a RequestHandler
	@param  string and dir_map
	*/
	RequestHandler(std::map<std::string, 
				   			http::server::handler_parameter>& dir_map);


	/**
	Handle a parsed HTTP request, and generates a response
	@param  request object
	@return response object
	*/
	virtual bool handleRequest(const request& req, std::string& response) = 0;

	virtual std::unique_ptr<http::server::Response> HandlerRequest(const request& request) = 0;

protected:
	// The map which map the URI to a specific path of the server
    std::map<std::string, http::server::handler_parameter> dir_map_;
};
}
}
#endif