#include <iostream>
#include <fstream>
#include <vector>

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
        std::string file_path = get_server_dir() + file_;
        std::ifstream is(file_path.c_str(), std::ios::in | std::ios::binary);
        if(!is) {
            // make the response as bad request
            response_->SetStatus(Response::bad_request);
            return response_;
        }
        response_->SetStatus(Response::ok);
        char buf[512];
        std::string fileContent = "";
        while(is.read(buf, sizeof(buf)).gcount() > 0) {
            fileContent.append(buf, is.gcount());
        }
        response_->AddHeader("Content-Length", std::to_string(fileContent.size()));
        response_->AddHeader("Content-type", mime_types::extension_to_type("html"));
        if(fileContent.empty()) {
            response_->SetStatus(Response::no_content);
        }
        response_->SetContent(fileContent);
        return response_;
    }
} // namespace server
} // namespace http
