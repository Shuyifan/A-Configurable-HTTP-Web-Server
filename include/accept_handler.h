#include "request_handler.h"
#include "request.h"
#include "header.h"
#include "mime_types.h"
#include "config_parser.h"

namespace http {
namespace server {
class AcceptHandler : public RequestHandler {
public:	
	static RequestHandler* create(const NginxConfig& config, 
								  const std::string& root_path);

	virtual std::unique_ptr<Response> HandlerRequest(const request& request) override;

    void setDir(std::string dir);
    void setRootDir(std::string dir);

private:
    std::string generateHTML(bool update, int id);
    void sanitizeInput(std::string& rawInput);
    void saveToFile(const std::string data, int id);
    std::string fileDir_;
    std::string rootDir_;
};
}
}