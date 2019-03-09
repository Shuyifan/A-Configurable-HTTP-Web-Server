#include <stdio.h>
#include<iostream>
#include <sstream>
#include <fstream>

#include "utils.h"
#include "stdlib.h"
#include <boost/filesystem.hpp>

std::string get_upper_dir(std::string str) {
    if(str.length() == 0) {
        return str;
    } else if(str.rfind("/") == -1) {
        return "";
    } else {
        return str.substr(0, str.rfind("/"));
    }
}

std::string get_server_dir() {
    std::string file_path = __FILE__;
    file_path = get_upper_dir(file_path);
    file_path = get_upper_dir(file_path);
    return file_path;
}

std::string get_file_name(std::string str, bool contain_extension) {
    if(str.rfind("/") == -1) {
        return "";
    } else {
        if(contain_extension) {
			return str.substr(str.rfind("/"));
		} else {
			std::string file_name = str.substr(str.rfind("/"));
			if(file_name.rfind(".") == -1) {
				return file_name;
			} else {
				return file_name.substr(0, file_name.rfind("."));
			}
		}
    }
}

bool url_decode(const std::string& in, std::string& out) {
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

void find_files_in_folder(std::string path, std::vector<std::string>& file_name) {
	boost::filesystem::path dir(path);
	if(boost::filesystem::exists(dir)) {
	    boost::filesystem::directory_iterator itEnd;
	    boost::filesystem::directory_iterator itDir(dir);
	    for(; itDir != itEnd; itDir++) {
	        file_name.push_back(itDir->path().string());
	    }
	}
}

// reference: https://stackoverflow.com/questions/5665231/most-efficient-way-to-escape-xml-html-in-c-string
void sanitizeInput(std::string& rawInput) {
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