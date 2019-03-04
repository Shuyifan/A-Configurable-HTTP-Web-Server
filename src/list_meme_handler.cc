#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

#include "utils.h"
#include "list_meme_handler.h"
#include <boost/filesystem.hpp>

namespace http {
namespace server {
    http::server::RequestHandler* ListMemeHandler::create(const NginxConfig& config, const std::string& root_path) {
        /*TODO:need to fill in the generatl functionality of create*/
        http::server::ListMemeHandler* handler = new http::server::ListMemeHandler();
        for(const auto& statement : config.statements_) {
			const std::vector<std::string> tokens = statement->tokens_;
			if(tokens[0] == "root") {
				handler->root_ = root_path + "/" + tokens[1];
			}
		}
        return handler;
    }

    std::unique_ptr<http::server::Response> ListMemeHandler::HandlerRequest(const request& request) {
        boost::filesystem::path dir(get_server_dir() + root_);
        std::vector<std::string> file_name;

        if(boost::filesystem::exists(dir)) {
            boost::filesystem::directory_iterator itEnd;
            boost::filesystem::directory_iterator itDir(dir);
            for(; itDir != itEnd; itDir++) {
                file_name.push_back(itDir->path().string());
            }
        }

        std::unique_ptr<Response> response_ (new Response);
        response_->SetVersion("1.1");

        std::string content = generateHTML(file_name);
        response_->AddHeader("Content-Length", std::to_string(content.size()));
        response_->AddHeader("Content-type", mime_types::extension_to_type("html"));
        
        if(content.empty()) {
            response_->SetStatus(Response::no_content);
        }

        response_->SetContent(content);
        return response_;
    }

    std::string ListMemeHandler::generateHTML(std::vector<std::string>& file_name) {
        std::stringstream ss;
        ss << "<!DOCTYPE html>";
        ss << "<html lang=\"en\" dir=\"ltr\">";
        ss << "<head>";
        ss << "<meta charset=\"utf-8\">";
        ss << "<title>Meme Generator</title>";
        ss << "<link rel=\"stylesheet\" href=\"https://stackpath.bootstrapcdn.com/bootstrap/4.3.1/css/bootstrap.min.css\" integrity=\"sha384-ggOyR0iXCbMQv3Xipma34MD+dH/1fQ784/j6cY/iJTQUOhcWr7x9JvoRxT2MZw1T\" crossorigin=\"anonymous\">";
        ss << "</head>";
        ss << "<body>";
        ss << "<table border=\"1\">";
        ss << "<tr>";
        ss << "<th>Month</th>";
        ss << "<th>Savings</th>";
        ss << "</tr>";
        ss << "<tr>";
        ss << "<td>January</td>";
        ss << "<td>$100</td>";
        ss << "</tr>";
        ss << "</table>";
        ss << "</body>";
        ss << "</html>";
        return ss.str();
    }
} // namespace server
} // namespace http
