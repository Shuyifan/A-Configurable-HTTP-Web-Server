#include <string>
#include <boost/asio.hpp>

#include "proxy_handler.h"
#include "request_handler.h"
#include "client.h"

namespace http {
namespace server {

ProxyHandler* ProxyHandler::create(const NginxConfig& config, const std::string& root_path)
{
	http::server::ProxyHandler* handler = new http::server::ProxyHandler();
		std::string url;
		for(const auto& statement : config.statements_) {
			const std::vector<std::string> tokens = statement->tokens_;
			if(tokens[0] == "location") {
				handler->baseUrl_ = url;
			}
			if(tokens[0] == "port") {
				handler->port_ = tokens[1];
			}
			if(tokens[0] == "address") {
				handler->host_ = tokens[1];
			}
		}

    return handler;
}

std::string ProxyHandler::extract_path_for_remote(std::string original_path)
{
	int base_length = baseUrl_.length();
	std::string new_path = original_path.substr(base_length);

	// make sure the new path begins with 1 backslash
	if(new_path[0] != '/')
		return "/" + new_path;

	// make sure not more than 1 backslash
	while (new_path.size() != 0 && new_path[0] == '/')
		new_path = new_path.substr(1);

	return new_path;
}

request ProxyHandler::setup_new_request(const request& old_request, std::string new_url)
{
	// NOTE!! Not sure if this is 100% correct
	// Make a new request object that has all the same values except for the new URL.
	// decided to keep the old request and new request separate.
	request new_request;
	new_request.uri = new_url;
	// copy old values that stayed the same
	new_request.method = old_request.method;
	new_request.http_version_major = old_request.http_version_major;
	new_request.http_version_minor = old_request.http_version_minor;
	new_request.headers = old_request.headers;

	return new_request;
}

std::unique_ptr<http::server::Response> ProxyHandler::HandlerRequest(const request& request)
{
	std::unique_ptr<http::server::Response> response_ (new http::server::Response);
	response_->SetVersion("1.1");

	// PART 1: Modify (if need be) the incoming Request to send to the Remote Server.
	// need to modify URL in request
	// Decode url to path.
    std::string original_request_path;

    if(!url_decode(request.uri, original_request_path)) {
		// make the response as bad request
		response_->SetStatus(http::server::Response::bad_request);
		return response_;
    }

	std::string new_request_path = extract_path_for_remote(original_request_path);

	request new_request = setup_new_request(request, new_request_path);
	// TODO: write out the new request to the remote server
	//Client client(new_request);
	//client.start(host_, port_);
	//client.set_request_string(new_Request.rawString());

	// PART 2: Construct the Response object to return.
	//std::string remote_response = client.read();
	// TODO: Should check to make sure Response is valid
	// TODO: parse Response object

	// TODO: If response_->GetStatus() == StatusCode::moved_temporarily:

	return response_;
}

} // server
} // http
