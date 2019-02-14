#include "response.h"

namespace http {
namespace server {
    std::string Response::resString(ResponseCode status) {
        switch (status) {
            case ResOK:
                return "OK";
            case ResBadReq:
                return "Bad Request";
            case ResNotFound:
                return "Not Found";
            case ResFound:
                return "Found";
            default:
                return "Bad Request";
        }
    }
    std::string Response::ToString() {
        headers["Content-Length"] = std::to_string(content.length());
        std::string response = "";
        response.append(" ");
        response.append(std::to_string((int)rescode));
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
        return response;
    }
    
}
}