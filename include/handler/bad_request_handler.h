#pragma once
#include "handler/request_handler.h"

namespace http {
namespace server {
/**
 * A handler to deal with error request. Will just return a 404 error page.
*/
class BadRequestHandler : public RequestHandler {
public:
	static RequestHandler* create(const NginxConfig& config, 
								  const std::string& root_path);

	virtual std::unique_ptr<http::server::Response> HandlerRequest(const request& request) override;
};
} // namespace server
} // namespace http

