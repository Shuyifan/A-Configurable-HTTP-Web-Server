#ifndef DEFAULT_HANDLER_H
#define DEFAULT_HANDLER_H
#include "request_handler.h"
#include "request.h"
#include "header.h"

namespace http {
namespace server {
class DefaultHandler : public RequestHandler {
public:

	DefaultHandler();
	/// Construct with a directory containing files to be served.
	DefaultHandler(std::map<std::string, 
	    				   http::server::handler_parameter>& dir_map);

	virtual bool handleRequest(const request& req, std::string& response) override;
};
}
}

#endif