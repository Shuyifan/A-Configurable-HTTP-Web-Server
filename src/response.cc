#include "response.h"
#include <iostream>
namespace http {
namespace server {

  
 

    std::string Response::resString(ResponseCode status) {
        switch (status) {

            case ok:
                return  "OK";
            case created:
                return  "Created";
            case accepted:
                return  "Accepted";
            case  no_content:
                return "No Content";
            case  multiple_choices:
                return  "Multiple Choices";
            case  moved_permanently:
                return  "Moved Permanently";
            case  moved_temporarily:
                return "Moved Temporarily";
            case  not_modified:
                return "Not Modified";
            case  bad_request:
                return "Bad Request";
            case  unauthorized:
                return "Unauthorized";
            case  forbidden:
                return  "Forbidden";
            case  not_found:
                return  "Not Found";
            case  internal_server_error:
                return  "Internal Server Error";
            case  not_implemented:
                return  "Not Implemented";
            case  bad_gateway:
                return  "Bad Gateway";;
            case  service_unavailable:
                return  "Service Unavailable";
            default:
                return  "Bad Request";
        }
    }
    std::string Response::ToString() {

        headers["Content-Length"] = std::to_string(content.length());
        std::string response = "";
        //response.append(" ");
        response.append("HTTP/1.0 "+ std::to_string((int)rescode));
        response.append(" ");
        response.append(resString(rescode));
        response.append("\r\n");
        for (auto p = headers.begin(); p != headers.end(); p++) {
            response.append(p->first);
            response.append(": ");
            response.append(p->second);
            response.append("\r\n");
        }
        response.append(content);
        std::cout << response;
        return response;
    }
    
    std::string Response::GetStringResult(ResponseCode status)
    {
        return resString(status);
    }
     
        
    
}
}