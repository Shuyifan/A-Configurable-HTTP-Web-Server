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
class ListMemeHandler : public RequestHandler {
public:	
	static RequestHandler* create(const NginxConfig& config, 
								  const std::string& root_path);

	virtual std::unique_ptr<Response> HandlerRequest(const request& request) override;

    

private:
    std::string root_;

	std::vector<NginxConfig> search_results_;

	std::string generateHTML(std::vector<std::string>& file_name);

	std::string sort_by_;

	void filterMeme(std::vector<std::string>& file_name);

	std::string getSortKey(const std::string uri);
};
}
}