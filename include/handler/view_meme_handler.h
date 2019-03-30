#include "handler/request_handler.h"
#include "request.h"
#include "header.h"
#include "mime_types.h"
#include "config_parser.h"

namespace http {
namespace server {
/**
 * A handler deals with view the generated meme. Given a URI request with id information inside,
 * return a response .
*/
class ViewMemeHandler : public RequestHandler {
public:
	static RequestHandler* create(const NginxConfig& config, 
								  const std::string& root_path);

	virtual std::unique_ptr<Response> HandlerRequest(const request& request) override;

    void setDir(std::string dir);

private:

    /**
     * Get the ID number from the request URI. (e.g. "/meme/view?id=12" -> 12)
     * @param uri The input uri
     * @return the id.
    */
    int getID(const std::string uri);

    std::string generateHTML(int id);

    // Working directory for the handler.
    std::string dir_;
};
}
}