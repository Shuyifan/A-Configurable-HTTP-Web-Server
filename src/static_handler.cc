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
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
namespace http {
namespace server {
StaticHandler::StaticHandler(const std::string& doc_root)
  : doc_root_(doc_root) {}

bool StaticHandler::handleRequest(const request& req, std::string& response) {
    std::stringstream res;

    // Decode url to path.
    std::string request_path;
    if(!url_decode(req.uri, request_path)) {
      //TODO:make the response as bad request
      //rep = reply::stock_reply(reply::bad_request);
      return false;
    }
    // Request path must be absolute and not contain "..".
    if(request_path.empty() || request_path[0] != '/'|| request_path.find("..") != std::string::npos) {
      //TODO:make the response as bad request
      //rep = reply::stock_reply(reply::bad_request);
      return false;
    }
    

	// Determine the file extension.
	std::size_t last_slash_pos = request_path.find_last_of("/");
	std::size_t last_dot_pos = request_path.find_last_of(".");
	std::string extension;
	if(last_dot_pos != std::string::npos && last_dot_pos > last_slash_pos) {
		extension = request_path.substr(last_dot_pos + 1);
	}

	// Open the file to send back.
	std::string full_path = doc_root_ + request_path;
	std::ifstream is(full_path.c_str(), std::ios::in | std::ios::binary);
	if(!is) {
		//TODO:make the response as bad request
		//rep = reply::stock_reply(reply::not_found);
		return false;
	}
	char buf[512];
	while (is.read(buf, sizeof(buf)).gcount() > 0) {
		res << "Content-Length";
		//res << std::to_string(rep.content.size());
		//TODO:put into the right format into the string
		res << mime_types::extension_to_type(extension);
		res << "Content-Type";
		res << "Content-Type";
		res << "HTTP/1.1 200 OK\r\n";
		res << "Content-Type: text/html\r\n";
		res << "Connection: close\r\n\r\n";
		res << req.method << " " << req.uri << " ";
		res << "HTTP/" << req.http_version_major << "." << req.http_version_minor << "\r\n";
		for(header hd : req.headers) {
			res << hd.name << ": " << hd.value << "\r\n";
		}
		res << "\r\n";
		response = res.str();
	}
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