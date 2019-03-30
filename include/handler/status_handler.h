#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include "handler_manager.h"

namespace http {
namespace server {

/**
 * A handler deals with providing the status information about the server.
 * The information include the number of request the server receieve, and the details of each request.
*/
class StatusHandler : public RequestHandler {
public:
    StatusHandler() {}

    static RequestHandler* create(const NginxConfig& config, 
								  const std::string& root_path);

    virtual std::unique_ptr<http::server::Response> HandlerRequest(const request& request) override;

private:

    // A vector to record all the handler's name and its corresponding URI they are dealing with.
    std::vector<std::pair<std::string, std::string>> handlers_;
    
    /**
     * Get the working directory of handler given its corresponding config.
     * e.g. 
     *  "handler static {
     *      location /foo;   -> "{base directory of the server}/foo"
     *      root static;
     *  }"
     * @param inner_config The handler's corresponding config.
    */
    std::string getLocation(std::unique_ptr<NginxConfig>& inner_config);
};

} // namespace server
} // namespace http


