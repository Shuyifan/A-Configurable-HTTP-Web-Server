// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// Copyright (c) 2012-2018, The CryptoNote developers, The Bytecoin developers.
// Licensed under the GNU Lesser General Public License. See LICENSE for details.
// https://github.com/bcndev/bytecoin/blob/master/src/http/ResponseParser.hpp

#ifndef RESPONSE_PARSER_H
#define RESPONSE_PARSER_H

#include "types.h"
#include "header.h"
#include "response.h"

namespace http {
namespace server {
//class Response; // forward declare

class ResponseParser {
	enum state {
		http_version_h,
		http_version_t_1,
		http_version_t_2,
		http_version_p,
		http_version_slash,
		http_version_major_start,
		http_version_major,
		http_version_minor_start,
		http_version_minor,
		status_code_1,
		status_code_2,
		status_code_3,
		status_code_space,
		status_text,
		expecting_newline_1,
		header_line_start,
		header_lws,
		header_name,
		space_before_header_value,
		header_value,
		expecting_newline_2,
		expecting_newline_3,
		good,
		bad
	} state_;

public:
	ResponseParser();

	void reset();

	state parse(Response& response, std::string response_string) {
		std::string::iterator it;
		for(it = response_string.begin(); it != response_string.end(); ++it)
		{
			if(state_ == good || state_ == bad)
				break;
			state_ = consume(response, *it);
		}
		if (state_ == good)
			convert_vector_to_map(response); // modify response to add temp_headers_ to headers map

		std::string body(it, response_string.end());
		if(body.size() > 0)
			response.SetContent(body);
		return state_;
	}
	bool is_good() const { return state_ == good; }
	bool is_bad() const { return state_ == bad; }

private:
	//bool process_ready_header(Response &response);
	//Header lowcase;
	state consume(Response &response, char input);
	std::vector<header> temp_headers_;
	void convert_vector_to_map(Response& response);

    int temp_status_code;
    std::string temp_version;
};

} // namespace server
} // namespace http

#endif
