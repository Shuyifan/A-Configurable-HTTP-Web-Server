#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

#include "utils.h"
#include "create_form_handler.h"

namespace http {
namespace server {
    RequestHandler* CreateFormHandler::create(const NginxConfig& config, 
								              const std::string& root_path) {
        CreateFormHandler* handler = new CreateFormHandler();
        for(const auto& statement : config.statements_) {
            const std::vector<std::string> tokens = statement->tokens_;
            if(tokens[0] == "root") {
                handler->setFile(root_path + "/" + tokens[1]);
                break;
            }
        }
        return handler;
    }

    void CreateFormHandler::setFile(std::string file) {
        file_ = file;
    }

    std::unique_ptr<Response> CreateFormHandler::HandlerRequest(const request& request) {
        std::unique_ptr<Response> response_ (new Response);
        response_->SetVersion("1.1");

        response_->SetStatus(Response::ok);
        std::string fileContent = generateHTML();

        response_->AddHeader("Content-Length", std::to_string(fileContent.size()));
        response_->AddHeader("Content-type", mime_types::extension_to_type("html"));
        
        if(fileContent.empty()) {
            response_->SetStatus(Response::no_content);
        }
        
        response_->SetContent(fileContent);
        return response_;
    }

    std::string CreateFormHandler::generateHTML() {
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
        ss << "<h2>Create your meme!</h2>";
        ss << "<form action=\"/meme/create\" method=\"post\">";
        ss << "<div class=\"form-group\">";
        ss << "<label>Select a template</label>";
        ss << "<select class=\"form-control\" name=\"image\" required=true>";
        ss << "<option value="">Select a template...</option>";
        
        std::vector<std::string> image_name;
        std::cout << get_server_dir() + file_ << std::endl;
        find_files_in_folder(get_server_dir() + file_, image_name);
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
        ss << "<input type=\"text\" class=\"form-control\" name=\"top\" placeholder=\"Top text...\" required=true>";
        ss << "</div>";
        ss << "<div class=\"form-group\">";
        ss << "<label for=\"exampleInputPassword1\">Bottom Text</label>";
        ss << "<input type=\"text\" class=\"form-control\" name=\"bottom\" placeholder=\"Bottom text...\" required=true>";
        ss << "</div>";
        ss << "<button type=\"submit\" class=\"btn btn-primary\">Create</button>";
        ss << "</form>";
        ss << "</div>";   
        ss << "</body>";
        ss << "</html>";

        return ss.str();
    }
} // namespace server
} // namespace http
