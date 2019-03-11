#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

#include "utils.h"
#include "search_handler.h"
#include <boost/filesystem.hpp>

namespace http {
namespace server {
    http::server::RequestHandler* SearchHandler::create(const NginxConfig& config, const std::string& root_path) {
        http::server::SearchHandler* handler = new http::server::SearchHandler();
        for(const auto& statement : config.statements_) {
			const std::vector<std::string> tokens = statement->tokens_;
			if(tokens[0] == "root") {
				handler->root_ = root_path + "/" + tokens[1];
			}
		}
        return handler;
    }

    std::unique_ptr<http::server::Response> SearchHandler::HandlerRequest(const request& request) {

        sort_by_ = getSortKey(request.uri);
        search_key_ = getSearchKey(request.uri);

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

    std::string SearchHandler::getSearchKey(const std::string uri) {
        int start = uri.find("q=") + 2;
        int end = start + 1;
        while(end < uri.size() && uri[end] != '?') {
            end++;
        }
        return uri.substr(start, end - start);
    }

    std::string SearchHandler::getSortKey(const std::string uri) {
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

    void SearchHandler::filterMeme(std::vector<std::string>& file_name) {
        NginxConfigParser config_parser;
        for(auto const& file: file_name) {
            NginxConfig config;
            config_parser.Parse(file.c_str(), &config);
            for(const auto& statement : config.statements_) {
                const std::vector<std::string> tokens = statement->tokens_;
                if(tokens[0] != "id") {
                    if(tokens[1].find(search_key_) != std::string::npos) {
                        search_results_.push_back(config);
                        break;
                    }
                }
            }
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

    std::string SearchHandler::generateHTML(std::vector<std::string>& file_name) {
        std::stringstream ss;
        ss << "<!DOCTYPE html>";
        ss << "<html lang=\"en\" dir=\"ltr\">";
        ss << "<head>";
        ss << "<meta charset=\"utf-8\">";
        ss << "<title>Search</title>";
        ss << "<link rel=\"stylesheet\" href=\"https://stackpath.bootstrapcdn.com/bootstrap/4.3.1/css/bootstrap.min.css\" integrity=\"sha384-ggOyR0iXCbMQv3Xipma34MD+dH/1fQ784/j6cY/iJTQUOhcWr7x9JvoRxT2MZw1T\" crossorigin=\"anonymous\">";
        ss << "</head>";
        ss << "<body>";
        ss << "<div class=\"container\">";
        ss << "<h2>Search Result</h2>";
        
        std::string decoded_key;
        url_decode(search_key_, decoded_key);

        if(search_results_.size() == 0) {
            ss << "No results found for " << "\"" << decoded_key << "\"";
        } else {
            ss << "Found " << search_results_.size() << " results for " << "\"" << decoded_key << "\"";
            ss << "<Br />";
            ss << "<Br />";
            ss << "<table class=\"table table-striped\">";
            ss << "<thead>";
            ss << "<tr>";
            ss << "<th scope=\"col\"><a href=\"?sort=id?q=" << search_key_ << "\">ID</th>";
            ss << "<th scope=\"col\"><a href=\"?sort=image?q=" << search_key_ << "\">Image </th>";
            ss << "<th scope=\"col\"><a href=\"?sort=top?q=" << search_key_ << "\">Top Text</th>";
            ss << "<th scope=\"col\"><a href=\"?sort=bottom?q=" << search_key_ << "\">Bottom Text</th>";
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
        }
        ss << "</body>";
        ss << "</html>";
        return ss.str();
    }
} // namespace server
} // namespace http
