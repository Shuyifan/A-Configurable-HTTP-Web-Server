#include "handler/request_handler.h"
#include "request.h"
#include "header.h"
#include "mime_types.h"
#include "config_parser.h"

namespace http {
namespace server {
/**
 * A handler to deal with actually creating the meme, and storing them into locals.
*/
class AcceptHandler : public RequestHandler {
public:	
	static RequestHandler* create(const NginxConfig& config, 
								  const std::string& root_path);

	virtual std::unique_ptr<Response> HandlerRequest(const request& request) override;

    void setDir(std::string dir);

    void setRootDir(std::string dir);

private:
    std::string generateHTML(bool update, int id);
    
    /**
     * Store the data into local.
     * @param data The data needed to be stored.
     * @param id The index for the saving file.
    */
    void saveToFile(const std::string data, int id);
    
    // The directory where the generated meme will store.
    std::string fileDir_;
    
    // The working directory for this handler.
    std::string rootDir_;
};
}
}