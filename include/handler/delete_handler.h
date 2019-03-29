#include "handler/request_handler.h"
#include "mime_types.h"
#include "config_parser.h"

namespace http {
namespace server {

class DeleteHandler : public RequestHandler {
public:
    DeleteHandler(std::string fileDir) : fileDir_(fileDir) {}

    static RequestHandler* create(const NginxConfig& config, 
								  const std::string& root_path);

    std::unique_ptr<Response> HandlerRequest(const request& request) override;

private:
    std::string generateHTML();
    int getID(const std::string uri);
    std::string fileDir_;
};

} // namespace server
} // namespace http
