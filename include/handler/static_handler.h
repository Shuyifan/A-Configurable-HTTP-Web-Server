#pragma once
#include <map>
#include "handler/request_handler.h"
#include "request.h"
#include "header.h"
#include "mime_types.h"
#include "handler_parameter.h"
#include "config_parser.h"

namespace http {
namespace server {
/**
 * A handler deals with serving the static files.
*/
class StaticHandler : public RequestHandler {
public:	
	static RequestHandler* create(const NginxConfig& config, 
								  const std::string& root_path);

	virtual std::unique_ptr<http::server::Response> HandlerRequest(const request& request) override;

	void setParameter(std::string url, http::server::handler_parameter param);

private:
	// Map each handler's name to their corresponding handler parameter.
	std::map<std::string, http::server::handler_parameter> param;
};
}
}
