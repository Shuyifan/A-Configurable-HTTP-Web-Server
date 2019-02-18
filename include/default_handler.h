#ifndef DEFAULT_HANDLER_H
#define DEFAULT_HANDLER_H
#include "request_handler.h"
#include "request.h"
#include "header.h"
#include "config_parser.h"

namespace http {
namespace server {
class DefaultHandler : public RequestHandler {
public:

	/**
	Create a echo handler according to the config file and root path.
	@param  NgixnConfig file and root path
	@return RequestHandler object that contains default handler
	*/
	static RequestHandler* create(const NginxConfig& config, 
								  const std::string& root_path);

	/**
	Given a request, this function will prepare a Response object 
	@param  request obejct
	@return unique pointer of Response
	*/
	virtual std::unique_ptr<http::server::Response> HandlerRequest(const request& request) override;
};
}
}

#endif