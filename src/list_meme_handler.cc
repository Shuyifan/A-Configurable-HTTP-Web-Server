#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>

#include "config_parser.h"
#include "utils.h"
#include "list_meme_handler.h"
#include <boost/filesystem.hpp>

namespace http {
namespace server {
    http::server::RequestHandler* ListMemeHandler::create(const NginxConfig& config, const std::string& root_path) {
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

        sort_by_ = getSortKey(request.uri);

        std::vector<std::string> file_name;
        find_files_in_folder(get_server_dir() + root_, file_name);

        std::unique_ptr<Response> response_ (new Response);
        response_->SetVersion("1.1");

        filterMeme(file_name);
        std::string content = generateHTML(file_name);

        response_->AddHeader("Content-Length", std::to_string(content.size()));
        response_->AddHeader("Content-type", mime_types::extension_to_type("html"));
        
        if(content.empty()) {
            response_->SetStatus(Response::no_content);
        } else {
            response_->SetStatus(Response::ok);
        }

        response_->SetContent(content);
        return response_;
    }

    void ListMemeHandler::filterMeme(std::vector<std::string>& file_name) {
        NginxConfigParser config_parser;
        for(auto const& file: file_name) {
            NginxConfig config;
            config_parser.Parse(file.c_str(), &config);
            search_results_.push_back(config);
        }
        if(sort_by_ == "id") {
            std::sort(search_results_.begin(), search_results_.end(), id_compare);
        } else if(sort_by_ == "image") {
            std::sort(search_results_.begin(), search_results_.end(), image_compare);
        } else if(sort_by_ == "top") {
            std::sort(search_results_.begin(), search_results_.end(), top_compare);
        } else if(sort_by_ == "bottom") {
            std::sort(search_results_.begin(), search_results_.end(), bottom_compare);
        }
    }

    std::string ListMemeHandler::getSortKey(const std::string uri) {
        if(uri.find("sort=") == std::string::npos) {
            return "id";
        }
        int start = uri.find("sort=") + 5;
        int end = start + 1;
        while(end < uri.size() && uri[end] != '?') {
            end++;
        }
        return uri.substr(start, end - start);
    }

    std::string ListMemeHandler::generateHTML(std::vector<std::string>& file_name) {
        std::stringstream ss;
        ss << "<!DOCTYPE html>";
        ss << "<html lang=\"en\" dir=\"ltr\">";
        ss << "<head>";
        ss << "<meta charset=\"utf-8\">";
        ss << "<title>Meme List</title>";
        ss << "<link rel=\"stylesheet\" href=\"https://stackpath.bootstrapcdn.com/bootstrap/4.3.1/css/bootstrap.min.css\" integrity=\"sha384-ggOyR0iXCbMQv3Xipma34MD+dH/1fQ784/j6cY/iJTQUOhcWr7x9JvoRxT2MZw1T\" crossorigin=\"anonymous\">";
        ss << "</head>";
        ss << "<body>";
        ss << "<div class=\"container\">";
        ss << "<h2>The Meme List</h2>";
        ss << "<form action=\"/meme/search\" method=\"get\">";
        ss << "<div class=\"form-group\">";
        ss << "<input type=\"text\" class=\"form-control\" name=\"q\" placeholder=\"Search\">";
        ss << "</div>";
        ss << "<button type=\"submit\" class=\"btn btn-primary\" value=\"Search\">Search</button>";
        ss << "</form>";
        ss << "<table class=\"table table-striped\">";
        ss << "<thead>";
        ss << "<tr>";
        ss << "<th scope=\"col\"><a href=\"?sort=id\">ID</th>";
        ss << "<th scope=\"col\"><a href=\"?sort=image\">Image </th>";
        ss << "<th scope=\"col\"><a href=\"?sort=top\">Top Text</th>";
        ss << "<th scope=\"col\"><a href=\"?sort=bottom\">Bottom Text</th>";
        ss << "</tr>";
        ss << "<tbody>";
        for(auto const& result: search_results_) {
            ss << "<tr>";
            for(const auto& statement : result.statements_) {
                const std::vector<std::string> tokens = statement->tokens_;
                std::string decoded_text;
                url_decode(tokens[1], decoded_text);
                sanitizeInput(decoded_text);
                if(tokens[0] == "id") {
                    ss << "<th scope=\"row\">" << "<a href=\"view?id=" << decoded_text << "\">" << decoded_text << "</a>" << "</th>";
                } else {
                    ss << "<td>" << decoded_text << "</td>";
                }
            }
            ss << "</tr>";
        }
        ss << "</tbody>";
        ss << "</thead>";
        ss << "</table>";
        ss << "</div>";
        ss << "</body>";
        ss << "</html>";
        return ss.str();
    }
} // namespace server
} // namespace http
