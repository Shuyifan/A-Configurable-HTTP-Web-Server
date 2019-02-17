#ifndef STATIC_HANDLER_H
#define STATIC_HANDLER_H
#include <map>
#include "request_handler.h"
#include "request.h"
#include "header.h"
#include "mime_types.h"
#include "handler_parameter.h"
#include "config_parser.h"

namespace http {
namespace server {
class StaticHandler : public RequestHandler {
public:
	StaticHandler();
	/// Construct with a directory containing files to be served.
	StaticHandler(std::map<std::string, 
	    				   http::server::handler_parameter>& dir_map);
	static RequestHandler* create(const NginxConfig& config, 
								  const std::string& root_path);


	virtual bool handleRequest(const request& req, std::string& response) override;
	/// Perform URL-decoding on a string. Returns false if the encoding was
	/// invalid.
	static bool url_decode(const std::string& in, std::string& out);

	virtual std::unique_ptr<http::server::Response> HandlerRequest(const request& request) override;

	void setParameter(std::string url, http::server::handler_parameter param);

private:
	std::map<std::string, http::server::handler_parameter> param;

};
}
}

#endif