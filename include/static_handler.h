#ifndef STATIC_HANDLER_H
#define STATIC_HANDLER_H
#include "request_handler.h"
#include "request.h"
#include "header.h"
#include "mime_types.h"
namespace http {
namespace server {
class StaticHandler : public RequestHandler {
public:
	StaticHandler() {}
	/// Construct with a directory containing files to be served.
	explicit StaticHandler(const std::string& doc_root);
	
	virtual bool handleRequest(const request& req, std::string& response) override;
	/// Perform URL-decoding on a string. Returns false if the encoding was
	/// invalid.
	static bool url_decode(const std::string& in, std::string& out);
	
private:
	/// The directory containing the files to be served.
	std::string doc_root_;
};
}
}

#endif