#pragma once
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
	RequestHandler() {};
	
	/**
	Handle a parsed HTTP request, and generates a response
	@param  request object
	@return response object
	*/
	//virtual bool handleRequest(const request& req, std::string& response) = 0;

	virtual std::unique_ptr<http::server::Response> HandlerRequest(const request& request) = 0;

};
}
}
