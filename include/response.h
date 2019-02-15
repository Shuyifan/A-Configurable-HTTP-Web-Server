#ifndef HTTP_RESPONSE_H
#define HTTP_RESPONSE_H

#include <string>
#include <vector>
#include <map>
#include "header.h"


namespace http {
namespace server {
    class Response {
 
        public:
        enum ResponseCode {
            ok = 200,
            created = 201,
            accepted = 202,
            no_content = 204,
            multiple_choices = 300,
            moved_permanently = 301,
            moved_temporarily = 302,
            not_modified = 304,
            bad_request = 400,
            unauthorized = 401,
            forbidden = 403,
            not_found = 404,
            internal_server_error = 500,
            not_implemented = 501,
            bad_gateway = 502,
            service_unavailable = 503
        };
        void SetStatus(const ResponseCode response_code) {
            rescode = response_code;
        }
        ResponseCode GetStatus(){
            return rescode;
        }
        std::string GetStringResult(ResponseCode status);
           
        void AddHeader(const std::string& name, const std::string& value) {
            headers[name] = value;
        }
        void SetContent(const std::string& body) {
            content = body;
        }

        void SetMime(const std::string& mime){
            mime_types = mime;
        }

        std::string ToString();

        private:
            std::string mime_types;
            std::map<std::string, std::string> headers;
            std::string content;
            ResponseCode rescode;
            //std::string string_result;
            std::string resString(ResponseCode status);
        
    };
}
}

#endif