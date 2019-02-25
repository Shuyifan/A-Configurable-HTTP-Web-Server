#pragma once
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
	static RequestHandler* create(const NginxConfig& config, 
								  const std::string& root_path);

	virtual std::unique_ptr<http::server::Response> HandlerRequest(const request& request) override;

	void setParameter(std::string url, http::server::handler_parameter param);

private:
	std::map<std::string, http::server::handler_parameter> param;

};
}
}
