#include "handler/request_handler.h"
#include "mime_types.h"
#include "config_parser.h"

namespace http {
namespace server {
/**
 * A handler deals with deleting a spcific generated meme.
*/
class DeleteHandler : public RequestHandler {
public:
    DeleteHandler(std::string fileDir) : fileDir_(fileDir) {}

    static RequestHandler* create(const NginxConfig& config, 
								  const std::string& root_path);

    std::unique_ptr<Response> HandlerRequest(const request& request) override;

private:
    std::string generateHTML();

    /**
     * Get the ID number from the request URI. (e.g. "/meme/view?id=12" -> 12)
     * @param uri The input uri
     * @return the id.
    */
    int getID(const std::string uri);
    
    // Working directory for the handler.
    std::string fileDir_;
};

} // namespace server
} // namespace http
