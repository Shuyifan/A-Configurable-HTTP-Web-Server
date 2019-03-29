#pragma once
#include "handler/request_handler.h"

namespace http {
namespace server {

class ProxyHandler : public RequestHandler {
public:
    ProxyHandler(const NginxConfig& config, const std::string& server_root) :
        handler_config_(config), server_root_(server_root) {}

	static RequestHandler* create(const NginxConfig& config,
                                  const std::string& root_path);

	virtual std::unique_ptr<Response> HandlerRequest(const request& request) override;

private:
 	std::string getDestURL(NginxConfig& config, std::string requestURL);
	std::string getPortNum(NginxConfig& config);
	std::string getHost(NginxConfig& config);
	NginxConfig handler_config_;
	std::string server_root_;
};

}
}