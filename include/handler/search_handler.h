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
 * A handler deals with searching the generated memes. 
*/
class SearchHandler : public RequestHandler {
public:	
	static RequestHandler* create(const NginxConfig& config, 
								  const std::string& root_path);

	virtual std::unique_ptr<Response> HandlerRequest(const request& request) override;

private:

	// The working directory of the handler.
    std::string root_;

	// The search key.
	std::string search_key_;

	// The key which the meme list is sorted based on。
	std::string sort_by_;

	// Get the search key (e.g. "/meme/search?key=aa" -> "aa").
	std::string getSearchKey(const std::string uri);

	// Get the sort key (e.g. "/meme/search?sort=aa" -> "aa").
	std::string getSortKey(const std::string uri);

	std::string generateHTML(std::vector<std::string>& file_name);

	// The vector contains the search result. Because the meme is also saved as the Ngix config 
	// form, so we used NginxConfig to save it.
	std::vector<NginxConfig> search_results_;

	/**
	 * Filter the memes which contains the search key.
	 * @param file_name A vector contains the files' namea.
	*/
	void filterMeme(std::vector<std::string>& file_name);
};
}
}