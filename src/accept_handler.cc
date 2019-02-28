#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <boost/filesystem.hpp>

#include "utils.h"
#include "accept_handler.h"

namespace http {
namespace server {
    RequestHandler* AcceptHandler::create(const NginxConfig& config, 
								          const std::string& root_path) {
        AcceptHandler* handler = new AcceptHandler();
        for(const auto& statement : config.statements_) {
            const std::vector<std::string> tokens = statement->tokens_;
            if(tokens[0] == "root") {
                handler->setFile(root_path + "/" + tokens[1]);
                break;
            }
        }
        return handler;
    }

    void AcceptHandler::setFile(std::string file) {
        file_ = file;
    }

    std::unique_ptr<Response> AcceptHandler::HandlerRequest(const request& request) {
        saveToFile(request.body);
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

    // reference: https://stackoverflow.com/questions/5665231/most-efficient-way-to-escape-xml-html-in-c-string
    void AcceptHandler::sanitizeInput(std::string& rawInput) {
        std::string buffer;
        buffer.reserve(rawInput.size());
        for(size_t pos = 0; pos != rawInput.size(); ++pos) {
            switch(rawInput[pos]) {
                case '&':
                    buffer.append("&amp;");
                    break;
                case '\"':
                    buffer.append("&quot;");
                    break;
                case '\'':
                    buffer.append("&apos;");
                    break;
                case '<':
                    buffer.append("&lt;");
                    break;
                case '>':
                    buffer.append("&gt;");
                    break;
                default:
                    buffer.append(&rawInput[pos], 1);
            }
        }
        rawInput.swap(buffer);
    }

    void AcceptHandler::saveToFile(const std::string body) {
        boost::filesystem::path dir("../files/userMemes");
        if(!(boost::filesystem::exists(dir))) {
            bool success = boost::filesystem::create_directory(dir);
            if(!success) {
                std::cout << "error in creating directory!\n";
                return;
            }
        }
        std::ofstream os("../files/userMemes/test");

        std::vector<std::string> fields = {"image", "top", "bottom"};
        std::vector<int> positions;
        for(int i = 0; i < fields.size(); i++) {
            positions.push_back(body.find(fields[i]) - 1);
        }
        positions.push_back(body.size());

        for(int i = 0; i < fields.size(); i++) {
            int start = positions[i] + 1;
            int end = positions[i + 1];
            int equalPos = body.find("=", start);
            std::string name = body.substr(start, equalPos - start);
            std::string value = body.substr(equalPos + 1, end - equalPos - 1);
            os << name << " " << value << ";\n";
        }
    }
} // namespace server
} // namespace http
