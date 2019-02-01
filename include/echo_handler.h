#ifndef ECHO_HANDLER_H
#define ECHO_HANDLER_H
#include "request_handler.h"
namespace http {
namespace server {
class EchoHandler : public RequestHandler {
 public:
  enum result_type { good, bad, indeterminate };
  //high level virtual function 
  virtual RequestHandler:: status HandleRequest(const request& req, 
          request* response);

};
}
}

#endif