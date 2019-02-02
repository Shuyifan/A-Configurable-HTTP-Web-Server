#ifndef ECHO_HANDLER_H
#define ECHO_HANDLER_H
#include "request_handler.h"
#include "request.h"
#include "header.h"

namespace http {
namespace server {
class EchoHandler : public RequestHandler {
public:
	EchoHandler() {}
	virtual bool handleRequest(const request& req, std::string& response) override;
};
}
}

#endif