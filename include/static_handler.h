#ifndef STATIC_HANDLER_H
#define STATIC_HANDLER_H
#include <map>
#include "request_handler.h"
#include "request.h"
#include "header.h"
#include "mime_types.h"
#include "handler_parameter.h"

namespace http {
namespace server {
class StaticHandler : public RequestHandler {
public:
	StaticHandler() {}
	/// Construct with a directory containing files to be served.
	explicit StaticHandler(std::map<std::string, 
								    http::server::handler_parameter>& dir_map);
	
	virtual bool handleRequest(const request& req, std::string& response) override;
	/// Perform URL-decoding on a string. Returns false if the encoding was
	/// invalid.
	static bool url_decode(const std::string& in, std::string& out);
	
private:
	// The map which map the URI to a specific path of the server
    std::map<std::string, http::server::handler_parameter> dir_map_;
};
}
}

#endif