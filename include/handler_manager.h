#pragma once
#include <memory>
#include <string>
#include <map>
#include <vector>
#include "config_parser.h"
#include "handler_parameter.h"
#include "handler/request_handler.h"
#include "handler/static_handler.h"
#include "handler/echo_handler.h"
#include "handler/default_handler.h"
#include "handler/error_handler.h"
#include "handler/status_handler.h"
#include "handler/create_form_handler.h"
#include "handler/view_meme_handler.h"
#include "handler/list_meme_handler.h"
#include "handler/accept_handler.h"
#include "handler/delete_handler.h"
#include "handler/health_handler.h"
#include "handler/bad_request_handler.h"
#include "handler/search_handler.h"
#include "handler/proxy_handler.h"

namespace http {
namespace server {
/**
 * A manager class to manage handler. Generated based on a config file.
 * Then it can store the relation between the each URI and the handler. Determine
 * when to use what handler. (e.g. /static - > staticHandler, /status -> statusHandler).
 * 
 * Besides, it also store the history information, including the request the server receieve and
 * the total number of them.
*/
class HandlerManager {
public:
	/**
     * Construct Handler Manager based on a config file.
     * @param config A config.
    */
	HandlerManager(const NginxConfig& config);
	
    /**
     * Create the needed type of handler given the name of handler, its corresponding config, and the 
     * root path for that handler.
     * @param name The name of handler (e.g. "static" -> StaticHandler).
     * @param config The config of the handler. (e.g. handler static {
                                                          location /foo;
                                                          root static;
                                                      }
                                                )
     * @param root_path The working direcotry for the handler 
     *                  (e.g. In the previous example, this will be 
     *                  "{base directory of the server}/foo")
     * @return the needed type of handler.
    */
	std::unique_ptr<http::server::RequestHandler> createByName(const std::string& name,
									 						   const NginxConfig& config,
                                     						   const std::string& root_path);
	/**
     * Create the needed type of handler given a URL ("/static" -> StaticHandler).
     * @param string url
     * @return the needed type of handler.
    */
	std::unique_ptr<http::server::RequestHandler> createByUrl(const std::string& url);

    static void addCount() {
        count ++;
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
     * Get the working directory of handler given its corresponding config.
     * e.g. 
     *  "handler static {
     *      location /foo;   -> "{base directory of the server}/foo"
     *      root static;
     *  }"
     * @param inner_config The handler's corresponding config.
    */
	std::string getLocation(std::unique_ptr<NginxConfig>& inner_config);
	
    //map the url to handler_factory_parameter
	std::map<std::string, http::server::handler_factory_parameter> param;
	
    //base directory location
	std::string base_dir;

    // Count the number of requests the server receives
    static int count;

    // The URL requested and the response status line
    static std::vector<std::pair<std::string, std::string>> reqResp;
};

}
}
