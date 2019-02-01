#ifndef REQUEST_HANDLER_H
#define  REQUEST_HANDLER_H
#include <string>
#include "request.h"
#include "config_parser.h"

namespace http {
namespace server {
class RequestHandler {
        public:
        //request handler status
        enum status { 
                OK = 0,
                error = 1
                };
        //high level function 

        // constructor
        static RequestHandler* createHandler(const char* type);

        // returns OK if successful
        // fileName = value in the config file
        virtual status init(const std::string& fileName, const NginxConfig& config) = 0;
  
        // Handles an HTTP request, and generates a response. Returns a response code
        virtual status HandleRequest(const request& req, request* response) = 0;

        


};
}
}
#endif