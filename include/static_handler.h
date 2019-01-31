#ifndef STATIC_HANDLER_H
#define STATIC_HANDLER_H
#include "request_handler.h"
namespace http {
namespace server {
class StaticHandler : public RequestHandler {
 public:
  enum result_type { good, bad, indeterminate };
  //high level virtual function 
  virtual RequestHandler:: result_type HandleRequest(const request& req, 
          request* response);

};
}
}

#endif