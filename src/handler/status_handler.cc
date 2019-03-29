#include <memory>
#include <sstream>
#include <vector>
#include <string>
#include "config_parser.h"
#include "handler/status_handler.h"
#include "handler_manager.h"

namespace http {
namespace server {
RequestHandler* StatusHandler::create(const NginxConfig& config, 
								       const std::string& root_path) {
    StatusHandler* handler = new StatusHandler();
    for(const auto& statement : config.statements_) {
        const std::vector<std::string> tokens = statement->tokens_;
        if(tokens[0] == "handler") {
            std::string handler_name = tokens[1];
            std::string url = handler->getLocation(statement->child_block_);
            handler->handlers_.push_back({handler_name, url});
        }
    }
    return handler;
}

std::unique_ptr<Response> StatusHandler::HandlerRequest(const request& request) {
    std::unique_ptr<Response> response_ (new Response);
    response_->SetVersion("1.1");
    response_->SetStatus(Response::ok);
    response_->AddHeader("Content-Type", "text/html");
    std::stringstream ss;
    ss << "<h3> Number of requests the server receives: </h3>";
    ss << HandlerManager::getCount();

    std::vector<std::pair<std::string, std::string>> resResp =
        HandlerManager::getReqResp();
    ss << "<h3> Request URLs received and responses returned: </h3>";
    ss << "<table border='1'>";
    ss << "<tr> <th>URL</th> <th>Response status</th>  </tr>";
    for(int i = 0; i < resResp.size(); i++) {
        ss << "<tr>";
        ss << "<td>" << resResp[i].first << "</td>";
        ss << "<td>" << resResp[i].second << "</td>";
        ss << "<tr>";
    }
    ss << "</table>";

    ss << "<h3> Existing handlers and corresponding URL prefixes </h3>";
    ss << "<table border='1'>";
    ss << "<tr> <th>Handler</th> <th>URL Prefix</th>  </tr>";
    for(int i = 0; i < handlers_.size(); i++) {
        ss << "<tr>";
        ss << "<td>" << handlers_[i].first << "</td>";
        ss << "<td>" << handlers_[i].second << "</td>";
        ss << "<tr>";
    }
    ss << "</table>";
    response_->SetContent(ss.str());
    return response_;
}

std::string StatusHandler::getLocation(std::unique_ptr<NginxConfig>& inner_config) {
    for(const auto& statement : inner_config->statements_) {
		const std::vector<std::string> tokens = statement->tokens_;
		if(tokens[0] == "location") {
			return tokens[1];
        }
	}
    return "";
}
} // namespace server
} // namespace http