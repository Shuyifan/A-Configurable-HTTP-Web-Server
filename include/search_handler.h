#pragma once
#include <vector>

#include "request_handler.h"
#include "request.h"
#include "header.h"
#include "mime_types.h"
#include "handler_parameter.h"
#include "config_parser.h"

namespace http {
namespace server {
class SearchHandler : public RequestHandler {
public:	
	static RequestHandler* create(const NginxConfig& config, 
								  const std::string& root_path);

	virtual std::unique_ptr<Response> HandlerRequest(const request& request) override;

    

private:
    std::string root_;

	std::string search_key_;

	std::string sort_by_;

	std::string getSearchKey(const std::string uri);

	std::string getSortKey(const std::string uri);

	std::string generateHTML(std::vector<std::string>& file_name);

	std::vector<NginxConfig> search_results_;

	void filterMeme(std::vector<std::string>& file_name);
};
}
}