#include "handler/request_handler.h"
#include "request.h"
#include "header.h"
#include "mime_types.h"
#include "handler_parameter.h"
#include "config_parser.h"

namespace http {
namespace server {
/**
 * A handler to deal with creating a meme, where the paage is for the users to interact with.
*/
class CreateFormHandler : public RequestHandler {
public:	
	static RequestHandler* create(const NginxConfig& config, 
								  const std::string& root_path);

	virtual std::unique_ptr<Response> HandlerRequest(const request& request) override;	

	void setImageDir(std::string dir);

	void setDataDir(std::string dir);

private:
	std::string generateHTML(bool update, int id);
    
	// The directory where the image templates store.
	std::string imageDir_;
	
	// The directory where the generated meme will store.
	std::string dataDir_;
};
}
}