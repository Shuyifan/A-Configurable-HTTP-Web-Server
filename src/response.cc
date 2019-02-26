#include "response.h"
#include <iostream>
#include <sstream>
namespace http {
namespace server {


    Response::StatusCode Response::getStatusCode(int status_code) {
        switch (status_code) {
            case 200:
                return ok;
            case 201:
                return created;
            case 202:
                return accepted;
            case  204:
                return no_content;
            case  300:
                return multiple_choices;
            case  301:
                return moved_permanently;
            case  302:
                return moved_temporarily;
            case  304:
                return not_modified;
            case  400:
                return bad_request;
            case  401:
                return unauthorized;
            case  403:
                return forbidden;
            case  404:
                return not_found;
            case  500:
                return internal_server_error;
            case  501:
                return not_implemented;
            case  502:
                return bad_gateway;
            case  503:
                return service_unavailable;
            default:
                return bad_request;
        }
    }

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
        response << "HTTP/" << version_ << " " << resString(status_code_);
        for (auto p = headers.begin(); p != headers.end(); p++) {
            response << p->first << ": ";
            response << p->second << "\r\n";
        }
        response << "\r\n";
        response << content_;
        return response.str();
    }   

    std::string Response::getStatusLine() {
        std::stringstream statusLine;
        statusLine << "HTTP/" << version_ << " " << resString(status_code_);
        return statusLine.str();
    }
    
} // namespace server
} // namespace http