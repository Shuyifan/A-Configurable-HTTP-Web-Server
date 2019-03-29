#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include "handler_manager.h"

namespace http {
namespace server {

class StatusHandler : public RequestHandler {
public:
    StatusHandler() {}
    static RequestHandler* create(const NginxConfig& config, 
								  const std::string& root_path);

    virtual std::unique_ptr<http::server::Response> HandlerRequest(const request& request) override;

private:
    std::vector<std::pair<std::string, std::string>> handlers_;
    std::string getLocation(std::unique_ptr<NginxConfig>& inner_config);
};

} // namespace server
} // namespace http


