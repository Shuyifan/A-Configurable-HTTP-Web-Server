#ifndef PROXY_HANDLER_H
#define PROXY_HANDLER_H
#include <boost/asio.hpp>
#include <memory>
#include "request_handler.h"
#include "client.h"

namespace http {
namespace server {
class ProxyHandler : public RequestHandler {
public:

	static ProxyHandler* create(const NginxConfig& config,
								  const std::string& root_path);

	virtual std::unique_ptr<http::server::Response> HandlerRequest(const request& request) override;

protected:

    static bool readConfig(const NginxConfig& config, ProxyHandler* handler);
    std::string extract_path_for_remote(std::string original_path);
    request setup_new_request(const request& old_request, std::string new_url);
    bool isRedirect(Response response);
    static void splitCombinedAddress(const std::string& combinedAddress, std::string& addressOut, std::string& locationOut, size_t startPosition=0);

    std::string location_;
	int port_;
    std::string address_;
    std::string prefix_;

};
} // namespace server
} // namespace http

#endif
