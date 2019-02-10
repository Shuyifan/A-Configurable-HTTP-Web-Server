#include <string>
#include "request_handler.h"
namespace http {
namespace server {

RequestHandler::RequestHandler() {}

RequestHandler::RequestHandler(std::map<std::string, 
				                        http::server::handler_parameter>& dir_map)
  : dir_map_(dir_map) {}
}
}