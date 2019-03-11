#pragma once
#include "request_handler.h"

namespace http {
namespace server {
class BadRequestHandler : public RequestHandler {
public:
	
	static RequestHandler* create(const NginxConfig& config, 
								  const std::string& root_path);

	virtual std::unique_ptr<http::server::Response> HandlerRequest(const request& request) override;
};
} // namespace server
} // namespace http
