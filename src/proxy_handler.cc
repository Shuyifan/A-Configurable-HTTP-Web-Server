#include <string>
#include <boost/asio.hpp>
#include <boost/log/trivial.hpp>
#include <memory>

#include "proxy_handler.h"
#include "request_handler.h"
#include "client.h"
#include "utils.h"
namespace http {
namespace server {

ProxyHandler* ProxyHandler::create(const NginxConfig& config, const std::string& root_path)
{
    BOOST_LOG_TRIVIAL(info) << "Creating proxy handler";
	http::server::ProxyHandler* handler = new http::server::ProxyHandler();
    bool success = readConfig(config, handler);
    if (!success) {
        BOOST_LOG_TRIVIAL(error) << "Error in reading proxy handler config; returning null ptr";
        return nullptr;
    }
    BOOST_LOG_TRIVIAL(trace) << "Returning created proxy handler";
    return handler;
}

std::unique_ptr<http::server::Response> ProxyHandler::HandlerRequest(const request& theRequest) {
    BOOST_LOG_TRIVIAL(trace) << "Handling proxy request";

    // determine url and hostname (address) for request to remote server
	std::string new_request_path = extract_path_for_remote(theRequest.uri);
    std::string new_address = address_;

    std::unique_ptr<Response> response;
    // Connect to remote server and receive response
    // If the response is a redirect, determine the new hostname/ url, and repeat
    // Exit if the response is not a redirect
    while (true) {
        request new_request = setup_new_request(theRequest, new_request_path);

        BOOST_LOG_TRIVIAL(trace) << "Setting up the client to the proxy server.  Address: " << new_address << "; Port: " << port_;
        std::unique_ptr<Client> client = createClient(new_address, port_);
        BOOST_LOG_TRIVIAL(trace) << "Getting response from the proxy server";
        response = client->getResponse(new_request);
        BOOST_LOG_TRIVIAL(trace) << "Received response from the proxy server:\n" << response->ToString();

        if (!isRedirect(*response))
            break;

        if (!response->headerExists("Location")) {
            BOOST_LOG_TRIVIAL(error) << "Redirect response contains no Location header.  Returning raw response to client.";
            return response;
        }
        BOOST_LOG_TRIVIAL(trace) << "Response from proxy server is a redirect.  Handling redirect...";
        std::string newCombinedAddress = response->getHeader("Location");
        splitCombinedAddress(newCombinedAddress, new_address, new_request_path);
    }
    BOOST_LOG_TRIVIAL(trace) << "Returning response from proxy server";

	return response;
}



std::unique_ptr<Client> ProxyHandler::createClient(std::string remoteAddress, int outgoingPort) {
    return Client::create(remoteAddress, outgoingPort);
}


std::string ProxyHandler::extract_path_for_remote(std::string original_path_raw)
{
	// Decode url to path.
    std::string original_path;
    if(!url_decode(original_path_raw, original_path)) {
		// mark the response as a bad request
	    std::unique_ptr<http::server::Response> errorResponse (new http::server::Response);
		errorResponse->SetStatus(http::server::Response::bad_request);
        BOOST_LOG_TRIVIAL(error) << "Reverse proxy handler:  Url could not be decoded.  Returning an empty path";
		return "";
    }

	int base_length = location_.length();
	std::string new_path = original_path.substr(base_length);

	// make sure the new path begins with 1 slash
	if(new_path[0] != '/')
		new_path = "/" + new_path;
    else {
        // make sure not more than 1 slash
        while (new_path.size() > 1 && new_path[0] == '/' && new_path[1] == '/')
            new_path = new_path.substr(1);
    }

    // prepend url prefix to new_path (both are guaranteed to start with a slash if nonempty)
    new_path = prefix_ + new_path;

	return new_path;
}

request ProxyHandler::setup_new_request(const request& old_request, std::string new_url) {
	// Make a new request object that has all the same values except for the new URL.
	request new_request(old_request);
	new_request.uri = new_url;
	return new_request;
}

bool ProxyHandler::isRedirect(Response response) {
    // This method allows easy extension for other redirects -- e.g., 303 and 307
    return (response.getStatus() == Response::moved_temporarily);
}

void ProxyHandler::splitCombinedAddress(const std::string& combinedAddress, std::string& addressOut, std::string& locationOut, size_t startPos) {
    // Split a combined address, e.g., http://www.ucla.edu/location/path, 
    //      into an address (http://www.ucla.edu) and a location (/location/path)
    //
    // Search for split point, which is first single '/'
    //      Be careful of possible preceding "//" -- e.g., "http://hostname/location"
    //
    //      Corner cases (in order to be checked) :
    //          * string is empty: entire string is (empty) location; set
    //                  location to '/' and leave addressOut unchanged
    //          * no '/': assume entire string is address, and location is empty (i.e., '/')
    //          * first '/' is last char: location is '/' (normal case)
    //          * first '/' immediately proceded by another '/': ignore both
    //          * '/' in first character: entire string is location; leave addressOut unchanged

    if ( combinedAddress.size()==0 ) { // string is empty
        locationOut = "/";
        // leave addressOut unchanged
        return;
    }
        
    std::size_t splitAtIndex=std::string::npos; // index of first '/'; the first character in the location
    splitAtIndex = combinedAddress.find('/', startPos);

    if ( splitAtIndex==std::string::npos ) { // no '/'
        addressOut = combinedAddress;
        locationOut = "/";
        return;
    }
    if ( splitAtIndex==combinedAddress.size()-1 ) { // first '/' is last char
        addressOut = combinedAddress.substr(0, combinedAddress.size()-1);
        locationOut = "/";
        return;
    }
    // first '/' immediately proceded by another '/' 
    if ( combinedAddress[splitAtIndex+1]=='/' ) {
        // ignore both
        splitCombinedAddress(combinedAddress, addressOut, locationOut, splitAtIndex+2);
        return;
    }
    // '/' in first character 
    if ( splitAtIndex==0 ) {
        // entire string is location; leave addressOut unchanged
        locationOut = combinedAddress;
        return;
    }
    // typical case -- '/' in the middle of the string
    addressOut = combinedAddress.substr(0, splitAtIndex);
    locationOut = combinedAddress.substr(splitAtIndex);
}


bool ProxyHandler::readConfig(const NginxConfig& config, ProxyHandler* handler) {

    // read config and set address config vars
    NginxConfig config_copy(config); // needed because ToString method isn't const
    BOOST_LOG_TRIVIAL(trace) << "Reading config:\n" << config_copy.ToString();
    
    // set defaults
    handler->prefix_ = "/";
    // ensure mandatory params are read
    bool location_read = false;
    bool address_read = false;
    bool port_read = false;

    std::string portString; // Mandatory

    for(const auto& statement : config.statements_) {
        const std::vector<std::string> tokens = statement->tokens_;
        if(tokens.size() >= 2 && tokens[0] == "location") {
            handler->location_ = tokens[1];
            location_read = true;
        } else if(tokens.size() >= 2 && tokens[0] == "address") {
            handler->address_ = tokens[1];
            address_read = true;
        } else if(tokens.size() >= 2 && tokens[0] == "port") {
            portString = tokens[1];
            port_read = true;
        } else if(tokens.size() >= 2 && tokens[0] == "prefix") {
            handler->prefix_ = tokens[1];
        }
    }
    if ( !( location_read && address_read && port_read ) ) {
        // fail -- required config keywords must be passed
        BOOST_LOG_TRIVIAL(error) << "Proxy handler -- Not all required config options were read:\n"
            << "\tLocation:  " << ( location_read ? "read" : "not read" ) << "\n"
            << "\tAddress:  "  << ( address_read  ? "read" : "not read" ) << "\n"
            << "\tPort:  "     << ( port_read     ? "read" : "not read" );
        return false;
    }

    // Convert port to positive integer
    bool is_valid = true;
    for (const char c: portString) {
        if (c<'0' || c>'9') {
            is_valid = false;
            break;
        }
    }
    if (!is_valid) {
        // fail -- port must be an integer
        BOOST_LOG_TRIVIAL(error) << "Proxy handler -- port in config is not an integer";
        return false;
    }
    int port = atoi(portString.c_str());
    if (port <= 0) {
        // fail -- port must be a positive integer
        BOOST_LOG_TRIVIAL(error) << "Proxy handler -- port in config is no a positive integer";
        return false;
    }
    handler->port_ = port;



    // ensure prefix_ does not end with a '/'
    while (handler->prefix_.size()>0 && handler->prefix_.back()=='/')
        handler->prefix_.pop_back();
    // ensure a non-empty prefix_ starts with a '/'
    if (handler->prefix_.size()>0 && handler->prefix_[0]!='/')
        handler->prefix_ = "/" + handler->prefix_;

    BOOST_LOG_TRIVIAL(trace) << "Proxy handler -- finished reading config."  
        << "  Location: " << handler->location_
        << "; Address: " << handler->address_ 
        << "; Port: " << handler->port_
        << "; Prefix: " << handler->prefix_;

    return true;
}

} // server
} // http
