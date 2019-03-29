//
// request_handler.cpp
// ~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "handler/static_handler.h"
#include "utils.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <map>
namespace http {
namespace server {

http::server::RequestHandler* StaticHandler::create(const NginxConfig& config, 
                                                    const std::string& root_path) {
    http::server::StaticHandler* handler = new http::server::StaticHandler();
		std::string url;
		http::server::handler_parameter temp_param;
		for(const auto& statement : config.statements_) {
			const std::vector<std::string> tokens = statement->tokens_;
			if(tokens[0] == "location") {
				url = tokens[1];
			} else if(tokens[0] == "root") {
				temp_param.dir = root_path + "/" + tokens[1];
			} else if(tokens[0] == "log") {
				temp_param.log = tokens[1];
			}
		}
		handler->setParameter(url, temp_param);
    return handler;
}

void StaticHandler::setParameter(std::string url, http::server::handler_parameter tempParam) {
	param[url] = tempParam;
}

std::unique_ptr<http::server::Response> StaticHandler::HandlerRequest(const request& request) {
	std::unique_ptr<http::server::Response> response_ (new http::server::Response);
	response_->SetVersion("1.1");

	// Decode url to path.
    std::string request_path;

    if(!url_decode(request.uri, request_path)) {
		// make the response as bad request
		response_->SetStatus(http::server::Response::bad_request);
		return response_;
    }

	// Request path must be absolute path and not contain "..".
	if(request_path.empty() || request_path[0] != '/' || request_path.find("..") != std::string::npos) {
		// make the response as bad request
		response_->SetStatus(http::server::Response::bad_request);
		return response_;
	}

	// Determine the file extension. (txt, jpg, html....)
	std::size_t last_slash_pos = request_path.find_last_of("/");
	std::size_t last_dot_pos = request_path.find_last_of(".");
	std::string extension;
	if(last_dot_pos != std::string::npos && last_dot_pos > last_slash_pos) {
		extension = request_path.substr(last_dot_pos + 1);
	}
	else { // file not exist
		response_->SetStatus(http::server::Response::not_found);
		response_->SetContent("File Not Exist!");
		return response_;
	}

	// Open the file to send back.
	std::string full_path = get_server_dir() + param.begin()->second.dir + "/" + get_file_name(request_path);
	std::ifstream is(full_path.c_str(), std::ios::in | std::ios::binary);
	if(!is) {
		// make the response as bad request
		response_->SetStatus(http::server::Response::bad_request);
		return response_;
	}
	response_->SetStatus(http::server::Response::ok);
	char buf[512];
	std::string fileContent = "";
	while(is.read(buf, sizeof(buf)).gcount() > 0) {
		fileContent.append(buf, is.gcount());
	}
	response_->AddHeader("Content-Length", std::to_string(fileContent.size()));
	response_->AddHeader("Content-type", mime_types::extension_to_type(extension));
	if(fileContent.empty()) {
		response_->SetStatus(http::server::Response::no_content);
	}
	response_->SetContent(fileContent);
	return response_;
}

}
}