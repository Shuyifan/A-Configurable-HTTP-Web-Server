#ifndef REQUEST_HANDLER_H
#define REQUEST_HANDLER_H
#include <string>
#include "request.h"

namespace http {
namespace server {
class RequestHandler {
public:
  
	// Handle a parsed HTTP request, and generates a response
	virtual bool handleRequest(const request& req, std::string& response) = 0;

};
}
}
#endif