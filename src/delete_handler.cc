#include <sstream>

#include <boost/filesystem.hpp>
#include "delete_handler.h"
#include "utils.h"

namespace http {
namespace server {

RequestHandler* DeleteHandler::create(const NginxConfig& config, 
								      const std::string& root_path) {
    std::string fileDir;
    for(const auto& statement : config.statements_) {
        const std::vector<std::string> tokens = statement->tokens_;
        if(tokens[0] == "root") {
            fileDir = root_path + "/" + tokens[1];
            break;
        }
    }
    DeleteHandler* handler = new DeleteHandler(fileDir);
    return handler;
}

std::unique_ptr<Response> DeleteHandler::HandlerRequest(const request& request) {
    std::unique_ptr<Response> response (new Response);
    response->SetVersion("1.1");
    response->AddHeader("Content-type", mime_types::extension_to_type("html"));

    int id = getID(request.uri);
    std::string targetFile = get_server_dir() + fileDir_ + "/meme" + std::to_string(id);
    if(!boost::filesystem::exists(targetFile)) {
        response->SetStatus(Response::not_found);
        response->SetContent("Invalid ID!");
        return response;
    }
    boost::filesystem::remove(targetFile);

    std::string content = generateHTML();
    response->SetStatus(Response::ok);
    response->AddHeader("Content-Length", std::to_string(content.size()));
    response->SetContent(content);
    return response;
}

int DeleteHandler::getID(std::string uri) {
    int pos = uri.find("id");
    int start = uri.find("=", pos) + 1;
    int end = start + 1;
    while(end < uri.size() && uri[end] != '?') {
        end++;
    }
    return std::stoi(uri.substr(start, end - start));
}

std::string DeleteHandler::generateHTML() {
    std::stringstream ss;
    ss << "<!DOCTYPE html>";
    ss << "<html lang=\"en\" dir=\"ltr\">";
    ss << "<head>";
    ss << "<meta charset=\"utf-8\">";
    ss << "<title>Delete Meme</title>";
    ss << "<link rel=\"stylesheet\" href=\"https://stackpath.bootstrapcdn.com/bootstrap/4.3.1/css/bootstrap.min.css\" integrity=\"sha384-ggOyR0iXCbMQv3Xipma34MD+dH/1fQ784/j6cY/iJTQUOhcWr7x9JvoRxT2MZw1T\" crossorigin=\"anonymous\">";
    ss << "</head>";
    ss << "<body>";
    ss << "<div class=\"container\">";
    ss << "<div class=\"jumbotron\">";
    ss << "<h2 class=\"display-4\">Delete Success!</h2>";
    ss << "<hr class=\"my-4\">";
    ss << "<p>To create a new meme, click Create.</p>";
    ss << "<p>To list all the memes created, click List.</p>";
    ss << "<a class=\"btn btn-primary btn-lg\" href=\"/meme/new\" role=\"button\">Create</a>";
    ss << "<a class=\"btn btn-info btn-lg\" href=\"/meme/list\" role=\"button\">List</a>";
    ss << "</div>";
    ss << "</div>";
    ss << "</body>";
    ss << "</html>";
    return ss.str();
}

}
}
