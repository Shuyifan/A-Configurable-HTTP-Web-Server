//
// request_handler.cpp
// ~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "static_handler.h"
#include "utils.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <map>
namespace http {
namespace server {

StaticHandler::StaticHandler() {}

StaticHandler::StaticHandler(std::map<std::string, 
																		  http::server::handler_parameter>& dir_map)
  : http::server::RequestHandler(dir_map) {}

http::server::RequestHandler* StaticHandler::create(const NginxConfig& config, 
                                                    const std::string& root_path) {
    http::server::StaticHandler* handler = new http::server::StaticHandler();
    return handler;
}

bool StaticHandler::handleRequest(const request& req, std::string& response) {
    std::stringstream res;

    // Decode url to path.
    std::string request_path;

    if(!url_decode(req.uri, request_path)) {
		// make the response as bad request
		res << "HTTP/1.1 400 Bad Request\r\n";
		res << "\r\n";
		response = res.str();
		return false;
    }
    // Request path must be absolute path and not contain "..".
    if(request_path.empty() || request_path[0] != '/' || request_path.find("..") != std::string::npos) {
		// make the response as bad request
		res << "HTTP/1.1 400 Bad Request\r\n";
		res << "\r\n";
		response = res.str();
		return false;
    }

	// Determine the file extension.
	std::size_t last_slash_pos = request_path.find_last_of("/");
	std::size_t last_dot_pos = request_path.find_last_of(".");
	std::string extension;
	if(last_dot_pos != std::string::npos && last_dot_pos > last_slash_pos) {
		extension = request_path.substr(last_dot_pos + 1);
	} else {
		// file not exist
		res << "HTTP/1.1 404 Not Found\r\n";
		res << "\r\n";
		res << "File Not Exist!";
		response = res.str();
		return false;
	}

	// Open the file to send back.
	std::string full_path = get_server_dir() + 
							dir_map_[get_upper_dir(request_path)].dir +
					     	get_file_name(request_path);
	std::ifstream is(full_path.c_str(), std::ios::in | std::ios::binary);
	if(!is) {
		// make the response as bad request
		res << "HTTP/1.1 404 Not Found\r\n";
		res << "\r\n";
		response = res.str();
		return false;
	}
	res << "HTTP/1.1 200 OK\r\n";
	char buf[512];
	std::string fileContent = "";
	while(is.read(buf, sizeof(buf)).gcount() > 0) {
		fileContent.append(buf, is.gcount());
	}
	res << "Content-Length: ";
	res << std::to_string(fileContent.size()) << "\r\n";
	res << "Content-type: ";
	res << mime_types::extension_to_type(extension) << "\r\n";
	res << "\r\n";
	res << fileContent;
	response = res.str();
	return true;
}

std::unique_ptr<http::server::Response> StaticHandler::HandlerRequest(const request& request) {
    return nullptr;
}

bool StaticHandler::url_decode(const std::string& in, std::string& out) {
	out.clear();
	out.reserve(in.size());
	for(std::size_t i = 0; i < in.size(); ++i) {
		if(in[i] == '%') {
			if(i + 3 <= in.size()) {
				int value = 0;
				std::istringstream is(in.substr(i + 1, 2));
				if (is >> std::hex >> value) {
					out += static_cast<char>(value);
					i += 2;
				} else {
					return false;
				}
			} else {
				return false;
			}
		} else if(in[i] == '+') {
			out += ' ';
		} else {
			out += in[i];
		}
	}
  	return true;
}


}
}