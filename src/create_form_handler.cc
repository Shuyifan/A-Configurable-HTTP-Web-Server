#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

#include <boost/filesystem.hpp>
#include "utils.h"
#include "create_form_handler.h"
namespace http {
namespace server {
    RequestHandler* CreateFormHandler::create(const NginxConfig& config, 
								              const std::string& root_path) {
        CreateFormHandler* handler = new CreateFormHandler();
        for(const auto& statement : config.statements_) {
            const std::vector<std::string> tokens = statement->tokens_;
            if(tokens[0] == "rootI") {
                handler->setImageDir(root_path + "/" + tokens[1]);
            }
            if(tokens[0] == "rootD") {
                handler->setDataDir(root_path + "/" + tokens[1]);
            }
        }
        return handler;
    }

    void CreateFormHandler::setImageDir(std::string imageDir) {
        imageDir_ = imageDir;
    }

    void CreateFormHandler::setDataDir(std::string dataDir) {
        dataDir_ = dataDir;
    }

    std::unique_ptr<Response> CreateFormHandler::HandlerRequest(const request& request) {
        std::unique_ptr<Response> response_ (new Response);

        bool update = false;
        int id;
        if(request.uri.find("update") != request.uri.npos) {
            update = true;
            int pos = request.uri.find("update");
            int start = request.uri.find("=", pos) + 1;
            int end = start + 1;
            while(end < request.uri.size() && request.uri[end] != '?') {
                end++;
            }
            id = std::stoi(request.uri.substr(start, end - start));
        }  

        response_->SetVersion("1.1");

        std::string fileContent = generateHTML(update, id);
        if(fileContent.find("Invalid") == fileContent.npos) {
            response_->SetStatus(Response::ok);
        } else {
            response_->SetStatus(Response::not_found);
        }

        response_->AddHeader("Content-Length", std::to_string(fileContent.size()));
        response_->AddHeader("Content-type", mime_types::extension_to_type("html"));
        
        if(fileContent.empty()) {
            response_->SetStatus(Response::no_content);
        }
        
        response_->SetContent(fileContent);
        return response_;
    }

    std::string CreateFormHandler::generateHTML(bool update, int id) {
        std::string top, bottom;
        if(update) {
            std::string targetFile = get_server_dir() + dataDir_ + "/meme" + std::to_string(id);
            if(!boost::filesystem::exists(targetFile)) {
                return "<h1> Invalid ID! </h1>";
            }
            NginxConfigParser config_parser;
            NginxConfig config;
            config_parser.Parse(targetFile.c_str(), &config);
            for(const auto& statement : config.statements_) {
                const std::vector<std::string> tokens = statement->tokens_;
                if(tokens[0] == "top") {
                    top = tokens[1];
                }
                if(tokens[0] == "bottom") {
                    bottom = tokens[1];
                }
            }
        }

        std::stringstream ss;
        
        ss << "<!DOCTYPE html>";
        ss << "<html lang=\"en\" dir=\"ltr\">";
        ss << "<head>";
        ss << "<meta charset=\"utf-8\">";
        ss << "<title>Meme Generator</title>";
        ss << "<link rel=\"stylesheet\" href=\"https://stackpath.bootstrapcdn.com/bootstrap/4.3.1/css/bootstrap.min.css\" integrity=\"sha384-ggOyR0iXCbMQv3Xipma34MD+dH/1fQ784/j6cY/iJTQUOhcWr7x9JvoRxT2MZw1T\" crossorigin=\"anonymous\">";
        ss << "</head>";
        ss << "<body>";
        ss << "<div class=\"container\">";
        if(update) {
            ss << "<h2>Edit the meme!</h2>";
        } else {
            ss << "<h2>Create your meme!</h2>";
        }
        ss << "<form action=\"/meme/create\" method=\"post\">";
        ss << "<div class=\"form-group\">";
        ss << "<label>Select a template</label>";
        ss << "<select class=\"form-control\" name=\"image\" required=true>";
        ss << "<option value="">Select a template...</option>";
        
        std::vector<std::string> image_name;
        std::cout << get_server_dir() + imageDir_ << std::endl;
        find_files_in_folder(get_server_dir() + imageDir_, image_name);
        for(auto const& image: image_name) {
            ss << "<option value=\"";
            ss << get_file_name(image).substr(1); 
            ss << "\">";
            ss << get_file_name(image, false).substr(1);
            ss << "</option>\"";
        }
        
        ss << "</select>";
        ss << "</div>";
        ss << "<div class=\"form-group\">";
        ss << "<label>Top Text</label>";
        if(update) {
            ss << "<input type=\"text\" class=\"form-control\" name=\"top\" value=\"" << top << "\" required=true>";
        } else {
            ss << "<input type=\"text\" class=\"form-control\" name=\"top\" placeholder=\"Top text...\" required=true>";
        }
        ss << "</div>";
        ss << "<div class=\"form-group\">";
        ss << "<label for=\"exampleInputPassword1\">Bottom Text</label>";
        if(update) {
            ss << "<input type=\"text\" class=\"form-control\" name=\"bottom\" value=\"" << bottom << "\" required=true>";
        } else {
            ss << "<input type=\"text\" class=\"form-control\" name=\"bottom\" placeholder=\"Bottom text...\" required=true>";
        }
        ss << "</div>";
        if(update) {
            ss << "<input type=\"hidden\" name=\"update\" value=\"" << id << "\">";
        }
        ss << "<button type=\"submit\" class=\"btn btn-primary\">Create</button>";
        ss << "</form>";
        ss << "</div>";   
        ss << "</body>";
        ss << "</html>";

        return ss.str();
    }
} // namespace server
} // namespace http
