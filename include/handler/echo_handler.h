#pragma once
#include "handler/request_handler.h"
#include "request.h"
#include "header.h"
#include "config_parser.h"

namespace http {
namespace server {
/**
 * A handler to perform the echo operation.
*/
class EchoHandler : public RequestHandler {
public:
	static RequestHandler* create(const NginxConfig& config, 
								  const std::string& root_path);

	virtual std::unique_ptr<http::server::Response> HandlerRequest(const request& request) override;
};
}
}

