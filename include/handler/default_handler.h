#pragma once
#include "handler/request_handler.h"
#include "request.h"
#include "header.h"
#include "config_parser.h"

namespace http {
namespace server {
/**
 * The defaut handler. If we do not find a handler to deal with the URI properly, we will
 * use this handler to handle that request.
*/
class DefaultHandler : public RequestHandler {
public:
	static RequestHandler* create(const NginxConfig& config, 
								  const std::string& root_path);

	virtual std::unique_ptr<http::server::Response> HandlerRequest(const request& request) override;
};
}
}

