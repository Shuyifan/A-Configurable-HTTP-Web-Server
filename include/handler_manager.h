#pragma once
#include <memory>
#include <string>
#include <map>
#include <vector>
#include "request_handler.h"
#include "static_handler.h"
#include "echo_handler.h"
#include "default_handler.h"
#include "error_handler.h"
#include "status_handler.h"
#include "create_form_handler.h"
#include "config_parser.h"
#include "handler_parameter.h"

namespace http {
namespace server {

class HandlerManager {
public:
	/**
    Construct Handler Manager, first save the path of base directory
	then map the url to handler_factory_parameter

    @param NginxConfig file
    @return 
    */
	HandlerManager(const NginxConfig& config);
	/**
    name represent what type of handler it is, reset the RequestHandler according to the name

    @param string name represent what type of handler it is
    @return unique_ptr RequestHandler
    */
	std::unique_ptr<http::server::RequestHandler> createByName(const std::string& name, 
									 						   const NginxConfig& config, 
                                     						   const std::string& root_path);
	/**
    Get the handler name of url by using param map and then call createByName

    @param string url
    @return unique_ptr RequestHandler
    */
	std::unique_ptr<http::server::RequestHandler> createByUrl(const std::string& url);

    static void addCount() {
        count++;
    }

    static int getCount() {
        return count;
    }

    static void addReqResp(std::string uri, std::string statusLine) {
        reqResp.push_back({uri, statusLine});
    }

    static std::vector<std::pair<std::string, std::string>> getReqResp() {
        return reqResp;
    }

private:
	/**
    Get the location of current file 

    @param string inner_config of config file
    @return string of current location parameter
    */
	std::string getLocation(std::unique_ptr<NginxConfig>& inner_config);
	//map the url to handler_factory_parameter
	std::map<std::string, http::server::handler_factory_parameter> param;
	//base directory location
	std::string base_dir;

    // Count the number of requests the server receives
    static int count;

    // URL requested and response status line
    static std::vector<std::pair<std::string, std::string>> reqResp;
};

}
}
