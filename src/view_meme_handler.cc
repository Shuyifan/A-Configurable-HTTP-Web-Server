#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

#include "utils.h"
#include "view_meme_handler.h"
#include "bad_request_handler.h"
namespace http {
namespace server {
    RequestHandler* ViewMemeHandler::create(const NginxConfig& config, 
                                            const std::string& root_path) {
        ViewMemeHandler* handler = new ViewMemeHandler();
        for(const auto& statement : config.statements_) {
            const std::vector<std::string> tokens = statement->tokens_;
            if(tokens[0] == "root") {
                handler->setDir(root_path + "/" + tokens[1]);
                break;
            }
        }
        return handler;
    }

    void ViewMemeHandler::setDir(std::string dir) {
        dir_ = dir;
    }

    std::unique_ptr<Response> ViewMemeHandler::HandlerRequest(const request& request) {
	    if(request.getStatus() == http::server::request::StatusCode::bad_request){
		    http::server::BadRequestHandler* handler  = new http::server::BadRequestHandler();
		    return handler->HandlerRequest(request);
        }
        int id = getID(request.uri);

        std::unique_ptr<Response> response_ (new Response);
        response_->SetVersion("1.1");

        std::string content = generateHTML(id);
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

    int ViewMemeHandler::getID(const std::string uri) {
        int pos = uri.find("id");
        int start = uri.find("=", pos) + 1;
        int end = start + 1;
        while(end < uri.size() && uri[end] != '?') {
            end++;
        }
        return std::stoi(uri.substr(start, end - start));
    }

    std::string ViewMemeHandler::generateHTML(int id) {
        std::string targetFile = get_server_dir() + dir_ + "/meme" + std::to_string(id);
        NginxConfigParser config_parser;
        NginxConfig config;
        config_parser.Parse(targetFile.c_str(), &config);
        std::string imageName, top, bottom;
        for(const auto& statement : config.statements_) {
            const std::vector<std::string> tokens = statement->tokens_;
            if(tokens[0] == "image") {
                imageName = tokens[1];
            }
            if(tokens[0] == "top") {
                top = tokens[1];
            }
            if(tokens[0] == "bottom") {
                bottom = tokens[1];
            }
        }

        std::stringstream ss;
        std::string decoded_top;
        std::string decoded_bottom;
        url_decode(top, decoded_top);
        url_decode(bottom, decoded_bottom);

        ss << "<!DOCTYPE html>";
        ss << "<html lang=\"en\" dir=\"ltr\">";
        ss << "<head>";
        ss << "<meta charset=\"utf-8\">";
        ss << "<title>Your Meme</title>";
        ss << "<link rel=\"stylesheet\" href=\"https://stackpath.bootstrapcdn.com/bootstrap/4.3.1/css/bootstrap.min.css\" integrity=\"sha384-ggOyR0iXCbMQv3Xipma34MD+dH/1fQ784/j6cY/iJTQUOhcWr7x9JvoRxT2MZw1T\" crossorigin=\"anonymous\">";
        ss << "<style>";
        ss << "#out { display: inline-block; position: relative; }";
        ss << "span { color: white; font: 2em bold Impact, sans-serif; position: absolute; text-align: center; width: 100%; }";
        ss << "#top { top: 0; }";
        ss << "#bottom { bottom: 0; }";
        ss << "#in { width: 576px; height: 421px; background: url(../files/images/" << imageName << "); }";
        ss << "</style>";
        ss << "</head>";
        ss << "<body>";
        ss << "<div id=\"out\">";
        ss << "<div id=\"in\">";
        ss << "<span id=\"top\">" << decoded_top << "</span>";
        ss << "<span id=\"bottom\">" << decoded_bottom << "</span>";
        ss << "</div>";
        ss << "</div>";
        ss << "<br>";
        ss << "<a class=\"btn btn-primary btn-lg\" href=\"/meme/new?update=" << id << "\" role=\"button\">Edit</a>";
        ss << "</body>";
        ss << "</html>";

        return ss.str();
    }

} // namespace server
} // namespace http