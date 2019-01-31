#ifndef REQUEST_HANDLER_H
#define  REQUEST_HANDLER_H
#include <string>
#include "request.h"
namespace http {
namespace server {
class RequestHandler {
 public:
  enum result_type { good, bad, indeterminate };
  //high level function 
  virtual result_type HandleRequest(const request& req, 
          request* response) = 0;

};
}
}
#endif