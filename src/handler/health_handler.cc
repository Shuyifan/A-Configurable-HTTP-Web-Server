#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

#include "config_parser.h"
#include "utils.h"
#include "handler/health_handler.h"
#include <boost/filesystem.hpp>

namespace http {
namespace server {
    http::server::RequestHandler* HealthHandler::create(const NginxConfig& config, const std::string& root_path) {
        http::server::HealthHandler* handler = new http::server::HealthHandler();
        return handler;
    }

    std::unique_ptr<http::server::Response> HealthHandler::HandlerRequest(const request& request) {

        std::unique_ptr<http::server::Response> response_ (new http::server::Response);
        response_->SetVersion("1.1");
        response_->SetStatus(http::server::Response::ok);
        response_->AddHeader("Content-Type", "text/html");
        std::stringstream res;
        res << "<h1>OK</h1>";
        response_->SetContent(res.str());
        return response_;
    }


} // namespace server
} // namespace http
