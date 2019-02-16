#include "response.h"
#include <iostream>
#include <sstream>
namespace http {
namespace server {

    std::string Response::resString(StatusCode status) {
        switch (status) {

            case ok:
                return  "200 OK\r\n";
            case created:
                return  "201 Created\r\n";
            case accepted:
                return  "202 Accepted\r\n";
            case  no_content:
                return "204 No Content\r\n";
            case  multiple_choices:
                return  "300 Multiple Choices\r\n";
            case  moved_permanently:
                return  "301 Moved Permanently\r\n";
            case  moved_temporarily:
                return "302 Moved Temporarily\r\n";
            case  not_modified:
                return "304 Not Modified\r\n";
            case  bad_request:
                return "400 Bad Request\r\n";
            case  unauthorized:
                return "401 Unauthorized\r\n";
            case  forbidden:
                return  "403 Forbidden\r\n";
            case  not_found:
                return  "404 Not Found\r\n";
            case  internal_server_error:
                return  "500 Internal Server Error\r\n";
            case  not_implemented:
                return  "501 Not Implemented\r\n";
            case  bad_gateway:
                return  "502 Bad Gateway\r\n";;
            case  service_unavailable:
                return  "503 Service Unavailable\r\n";
            default:
                return  "400 Bad Request\r\n";
        }
    }
    
    std::string Response::ToString() {
        std::stringstream response;
        response << "HTTP/" << version_ << resString(status_code_);
        for (auto p = headers.begin(); p != headers.end(); p++) {
            response << p->first << ": ";
            response << p->second << "\r\n";
        }
        response << "\r\n";
        response << content_;
        return response.str();
    }   
    
} // namespace server
} // namespace http