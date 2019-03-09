//
// request.hpp
// ~~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#pragma once

#include <string>
#include <vector>
#include "header.h"

namespace http {
namespace server {

/// A request received from a client.
struct request
{
  //this is are all the necearry information contain by request
  std::string method;
  std::string uri;
  int http_version_major;
  int http_version_minor;
  std::vector<header> headers;
  std::string body;

  void clear() {
    method = "";
    uri = "";
    headers.clear();
  }
  
  enum StatusCode {
        ok = 200,
        bad_request = 400,
  };
  StatusCode status_code_;
  StatusCode getStatusCode(int status_code) {
        switch (status_code) {
            case 200:
                return ok;
            case  400:
                return bad_request;

        }
  }


  void SetStatus(const StatusCode status_code) {
    status_code_ = status_code;
  }

  StatusCode GetStatus(){
    return status_code_;
  }
  StatusCode getStatus() const {
    return status_code_;
  }
  std::string rawString() {
    std::string request;
    request.append(method);
    request.append(" ");
    request.append(uri);
    request.append(" ");
    request.append("HTTP/");
    request.append(std::to_string(http_version_major));
    request.append(".");
    request.append(std::to_string(http_version_minor));
    request.append("\r\n");
    for(header hd : headers) {
        request.append(hd.name);
        request.append(": ");
        request.append(hd.value);
        request.append("\r\n");
    }
    request.append("\r\n");
    return request;
  }
  
};

} // namespace server
} // namespace http


