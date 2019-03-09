#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <boost/filesystem.hpp>

#include "utils.h"
#include "accept_handler.h"
namespace http {
namespace server {
    RequestHandler* AcceptHandler::create(const NginxConfig& config, 
								          const std::string& root_path) {
        AcceptHandler* handler = new AcceptHandler();
        handler->setRootDir(get_server_dir() + root_path);
        for(const auto& statement : config.statements_) {
            const std::vector<std::string> tokens = statement->tokens_;
            if(tokens[0] == "root") {
                handler->setDir(get_server_dir() + root_path + "/" + tokens[1]);
                break;
            }
        }
        return handler;
    }

    void AcceptHandler::setDir(std::string dir) {
        fileDir_ = dir;
    }

    void AcceptHandler::setRootDir(std::string dir) {
        rootDir_ = dir;
    }

    std::unique_ptr<Response> AcceptHandler::HandlerRequest(const request& request) {
        bool update = false;
        int id;
        if(request.body.find("update") != request.body.npos) {
            update = true;
            int pos = request.body.find("update");
            int start = request.body.find("=", pos) + 1;
            int end = start + 1;
            while(end < request.body.size() && request.body[end] != '?') {
                end++;
            }
            id = std::stoi(request.body.substr(start, end - start));
        } else {
            std::string idFile = rootDir_ + "/id";
            if(!boost::filesystem::exists(idFile)) {
                id = 1;
            } else {
                std::ifstream is(idFile);
                is >> id;
                is.close();
            }
        }

        std::unique_ptr<Response> response_ (new Response);
        response_->SetVersion("1.1");

        if(request.body.find("image") == request.body.npos || request.body.find("top") == request.body.npos || request.body.find("bottom") == request.body.npos) {
            response_->SetStatus(Response::not_found);
            response_->SetContent("<h1>Error!</h1>");
            response_->AddHeader("Content-type", mime_types::extension_to_type("html"));
            return response_;
        }

        saveToFile(request.body, id);

        std::string content = generateHTML(update, id);
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

    std::string AcceptHandler::generateHTML(bool update, int id) {
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
        ss << "<div class=\"jumbotron\">";
        ss << "<h2 class=\"display-4\">Success!</h2>";
        ss << "<hr class=\"my-4\">";
        ss << "<p>To create one more meme, click Create Another.</p>";
        ss << "<p>To view your created meme, click View.</p>";
        ss << "<p>To list all the memes created, click List.</p>";
        ss << "<a class=\"btn btn-primary btn-lg\" href=\"/meme/new\" role=\"button\">Create Another</a>";
        ss << "<a class=\"btn btn-success btn-lg\" href=\"/meme/view" << "?id=" << id << "\" " << "role=\"button\">View</a>";
        ss << "<a class=\"btn btn-info btn-lg\" href=\"/meme/list\" role=\"button\">List</a>";
        ss << "</div>";
        ss << "</div>";
        ss << "</body>";
        ss << "</html>";

        if(!update) {
            std::string idFile = rootDir_ + "/id";
            std::ofstream os(idFile);
            id++;
            os << id;
        }
        
        return ss.str();
    }

    void AcceptHandler::saveToFile(const std::string body, int id) {
        boost::filesystem::path dir(fileDir_);
        if(!(boost::filesystem::exists(dir))) {
            bool success = boost::filesystem::create_directory(dir);
            if(!success) {
                std::cout << "error in creating directory!\n";
                return;
            }
        }
        std::string toFile = fileDir_ + "/meme" + std::to_string(id);
        std::ofstream os(toFile);
        os << "id " << id << ";\n";

        std::vector<std::string> fields = {"image", "top", "bottom"};
        std::vector<int> positions;
        for(int i = 0; i < fields.size(); i++) {
            positions.push_back(body.find(fields[i]) - 1);
        }
        if(body.find("update") == body.npos) {
            positions.push_back(body.size());
        } else {
            positions.push_back(body.find("update") - 1);
        }

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