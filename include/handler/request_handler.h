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
/**
 * An interface of the handler. 
 * 
 * Besides the HandlerRequest function, each handler implementation must include a static function 
 * as follows:
 * 		static RequestHandler* create(const NginxConfig& config, const std::string& root_path)
 * where config is the config of the corresponding handler, and root_path is its working directory.
*/
class RequestHandler {
public:
	RequestHandler() {};
	/**
	 * Handle a parsed HTTP request, and generates a response
	 * @param  request object
	 * @return response object
	*/
	virtual std::unique_ptr<http::server::Response> HandlerRequest(const request& request) = 0;

};
}
}
