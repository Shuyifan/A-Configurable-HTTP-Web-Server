#ifndef HTTP_RESPONSE_H
#define HTTP_RESPONSE_H

#include <string>
#include <vector>
#include <map>
#include "header.h"

//FIXME: MISSING MIME TYPE (what is it?)

namespace http {
namespace server {
    class Response {
        public:
        enum ResponseCode {
            ResOK = 200,
            ResFound = 302,
            ResBadReq = 400,
            ResNotFound = 404
        };
        void SetStatus(const ResponseCode response_code) {
            rescode = response_code;
        }
        void AddHeader(const std::string& name, const std::string& value) {
            headers[name] = value;
        }
        void SetContent(const std::string& body) {
            content = body;
        }

        std::string ToString();
        private:
        std::map<std::string, std::string> headers;
        std::string content;
        ResponseCode rescode;
        std::string resString(ResponseCode status);
        
    };
}
}

#endif;