#pragma once
#include <vector>

#include "handler/request_handler.h"
#include "request.h"
#include "header.h"
#include "mime_types.h"
#include "handler_parameter.h"
#include "config_parser.h"

namespace http {
namespace server {
/**
 * A handler deals with listing all the generated memes. 
*/
class ListMemeHandler : public RequestHandler {
public:	
	static RequestHandler* create(const NginxConfig& config, 
								  const std::string& root_path);

	virtual std::unique_ptr<Response> HandlerRequest(const request& request) override;

private:

	// Working directory for the handler.
    std::string root_;

	// The vector contains the search result. Because the meme is also saved as the Ngix config 
	// form, so we used NginxConfig to save it.
	std::vector<NginxConfig> search_results_;

	std::string generateHTML(std::vector<std::string>& file_name);

	// The key which the meme list is sorted based on。
	std::string sort_by_;

	/**
	 * Filter the memes which contains the search key.
	 * @param file_name A vector contains the files' namea.
	*/
	void filterMeme(std::vector<std::string>& file_name);

	// Get the sort key (e.g. "/meme/search?sort=aa" -> "aa").
	std::string getSortKey(const std::string uri);
};
}
}