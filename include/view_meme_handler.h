#include "request_handler.h"
#include "request.h"
#include "header.h"
#include "mime_types.h"
#include "config_parser.h"

namespace http {
namespace server {
class ViewMemeHandler : public RequestHandler {
public:
	static RequestHandler* create(const NginxConfig& config, 
								  const std::string& root_path);

	virtual std::unique_ptr<Response> HandlerRequest(const request& request) override;

    void setDir(std::string dir);

private:
    int getID(const std::string uri);
    std::string generateHTML(int id);
    std::string dir_;
};
}
}