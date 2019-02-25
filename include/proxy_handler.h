#ifndef PROXY_HANDLER_H
#define PROXY_HANDLER_H
#include <boost/asio.hpp>
#include "request_handler.h"
#include "client.h"

namespace http {
namespace server {
class ProxyHandler : public RequestHandler {
public:

	static ProxyHandler* create(const NginxConfig& config,
								  const std::string& root_path);

	virtual std::unique_ptr<http::server::Response> HandlerRequest(const request& request) override;

private:
	std::string baseUrl_;
	int port_;
	std::string host_;
	boost::asio::io_service io_service_;
};
} // namespace server
} // namespace http

#endif
