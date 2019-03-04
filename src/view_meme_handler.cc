#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

#include "utils.h"
#include "view_meme_handler.h"

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
        int id = getID(request.uri);

        std::unique_ptr<Response> response_ (new Response);
        response_->SetVersion("1.1");

        std::string content = generateHTML(id);
        response_->AddHeader("Content-Length", std::to_string(content.size()));
        response_->AddHeader("Content-type", mime_types::extension_to_type("html"));
        if(content.empty()) {
            response_->SetStatus(Response::no_content);
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
        ss << "<!DOCTYPE html>";
        ss << "<html lang=\"en\" dir=\"ltr\">";
        ss << "<head>";
        ss << "<meta charset=\"utf-8\">";
        ss << "<title>Your Meme</title>";
        ss << "<style>";
        ss << "body { display: inline-block; position: relative; }";
        ss << "span { color: white; font: 2em bold Impact, sans-serif; position: absolute; text-align: center; width: 100%; }";
        ss << "#top { top: 0; }";
        ss << "#bottom { bottom: 0; }";
        ss << "div { width: 576px; height: 421px; background: url(../files/images/" << imageName << "); }";
        ss << "</style>";
        ss << "</head>";
        ss << "<body>";
        ss << "<div>";
        //ss << "<img src=\"../files/images/" << imageName << "\" alt=\"" << imageName << "\">";
        ss << "<span id=\"top\">" << top << "</span>";
        ss << "<span id=\"bottom\">" << bottom << "</span>";
        ss << "</div>";
        ss << "</body>";
        ss << "</html>";

        return ss.str();
    }

} // namespace server
} // namespace http