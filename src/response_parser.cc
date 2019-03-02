// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// Copyright (c) 2012-2018, The CryptoNote developers, The Bytecoin developers.
// Licensed under the GNU Lesser General Public License. See LICENSE for details.

#include <sstream>
#include "response_parser.h"
#include "response.h"
#include "header.h"

namespace http {
namespace server {

ResponseParser::ResponseParser() : state_(http_version_h)
{
	temp_status_code = 0;
	temp_version = "";
}

void ResponseParser::reset() {
	state_  = http_version_h;
}

ResponseParser::state ResponseParser::consume(Response& response, char input)
{
	switch (state_) {
	case http_version_h:
		if (input == 'H')
			return http_version_t_1;
		return bad;
	case http_version_t_1:
		if (input == 'T')
			return http_version_t_2;
		return bad;
	case http_version_t_2:
		if (input == 'T')
			return http_version_p;
		return bad;
	case http_version_p:
		if (input == 'P')
			return http_version_slash;
		return bad;
	case http_version_slash:
		if (input == '/')
			return http_version_major_start;
		return bad;
	case http_version_major_start:
		if (is_digit(input)) {
			temp_version.push_back(input);
			// changed because Response version_ is string, not numeric
			return http_version_major;
		}
		return bad;
	case http_version_major:
		if (input == '.')
			return http_version_minor_start;
		if (is_digit(input)) {
			temp_version.push_back(input);
			//response.version_.push_back(input);
			return http_version_major;
		}
		return bad;
	case http_version_minor_start:
		if (is_digit(input)) {
			temp_version.push_back('.'); // since minor start, add a period
			temp_version.push_back(input);
			return http_version_minor;
		}
		return bad;
	case http_version_minor:
		if (input == ' ') {
			response.SetVersion(temp_version); // everything added to temp_version
			return status_code_1;
		}
		if (is_digit(input)) {
			temp_version.push_back(input);
			return http_version_minor;
		}
		return bad;
	case status_code_1:
		if (is_digit(input)) {
			temp_status_code = temp_status_code * 10 + input - '0';
			return status_code_2;
		}
		return bad;
	case status_code_2:
		if (is_digit(input)) {
			temp_status_code = temp_status_code * 10 + input - '0';
			return status_code_3;
		}
		return bad;
	case status_code_3:
		if (is_digit(input)) {
			temp_status_code = temp_status_code * 10 + input - '0';
			response.SetStatus(response.getStatusCode(temp_status_code));
			return status_code_space;
		}
		return bad;
	case status_code_space:
		if (input == ' ')
			return status_text;
		return bad;
	case status_text:
		if (input == '\r')
			return expecting_newline_1;
		if (is_ctl(input))
			return bad;
		return status_text;
	case expecting_newline_1:
		if (input == '\n') {
			temp_headers_.reserve(20);
			return header_line_start;
		}
		return bad;
	case header_line_start:
		if (input == '\r')
			return expecting_newline_3;
		if (!temp_headers_.empty() && (input == ' ' || input == '\t'))
			return header_lws;
		if (!is_char(input) || is_ctl(input) || is_tspecial(input))
			return bad;
		temp_headers_.push_back(header{});
		temp_headers_.back().name.reserve(16);
		temp_headers_.back().value.reserve(16);
		temp_headers_.back().name.push_back(input);
		return header_name;
	case header_lws:
		if (input == '\r')
			return expecting_newline_2;
		if (input == ' ' || input == '\t')
			return header_lws;
		if (is_ctl(input))
			return bad;
		temp_headers_.back().value.push_back(input);
		return header_value;
	case header_name:
		if (input == ':')
			return space_before_header_value;
		if (!is_char(input) || is_ctl(input) || is_tspecial(input))
			return bad;
		temp_headers_.back().name.push_back(input);
		return header_name;
	case space_before_header_value:
		if (input == ' ')
			return header_value;
		return bad;
	case header_value:
		if (input == '\r') {
			return expecting_newline_2;
		}
		if (is_ctl(input))
			return bad;
		temp_headers_.back().value.push_back(input);
		return header_value;
	case expecting_newline_2:
		if (input == '\n')
			return header_line_start;
		return bad;
	case expecting_newline_3:
		if (input == '\n')
			return good;
		return bad;
	default:
		return bad;
	}
}

void ResponseParser::convert_vector_to_map(Response& response)
{
	std::vector<header>::iterator it;
	// loop over all entries inside temp_headers_ and add it to the headers map
	for (const auto& elem: temp_headers_)
	{
		response.AddHeader(elem.name, elem.value);
		//response.headers[elem.name] = elem.value;
	}
}

}
}
