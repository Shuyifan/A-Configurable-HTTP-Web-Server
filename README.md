# cracking-the-web
Our poject is a webserver which can handle echo, static file,and other ruquest in c++
### A typical top-level directory layout

    .
    ├── build                   # Compiled files (alternatively `dist`)
    ├── conf                    # deploy.conf file is inside
    ├── docker                  # Docker information
    ├── files                   # Contains Static files inside the folder
    ├── include                 # Contains all the header files
    ├── src                     # Contains all .cc files
    ├── test                    # Contains unit tests and integration test
    └── README.md
### Information about header file
    .
    ├── ...
    ├── include                 # Contains all the header files
    │   ├── config_parser.h     # Take a opened config file or file name and store the parsed config in the provided NginxConfig out-param
    │   ├── request_handler.h   # Interface class of all the handler
    │   ├── default_handler.h   # Handle the request to main page
    │   ├── echo_handler.h      # Handle the echo request
    │   ├── handler_manager.h   # Prepare the correct request handler
    │   ├── handler_parameter.h # Contains directory and log as well as handler name and config
    │   ├── header.h            # Contains string called name and value
    │   ├── mime_types.h        # Convert a file extension into a MIME type.
    │   ├── request_parser.h    # Parse the request
    │   ├── request.h           # Request object
    │   ├── response.h          # Response object
    │   ├── server.h            # For server
    │   ├── session.h           # For session
    │   └── utils.h             # Get the correct directory
    └── ...
## Getting Started
### Installing 
```
 git clone ssh://${username}@code.cs130.org:29418/cracking-the-web.git
```
### Build locally
First change the conf/deploy.conf listen 80 to listen 8080
```
cd build
cmake ..
make 
make test
```
### Docker Build
```
tools/env/start.sh -u ${username}
cd cracking-the-web
./temp.sh 
docker run --rm -p 127.0.0.1:8080:8080 --name my_run my_image:latest
```
### Checking Website
-Main page:<a href="http://localhost:8080/" target="_blank">Here</a>.
-Static index:<a href="http://localhost:8080/static/index.html" target="_blank">Here</a>.
-Echo:<a href="http://localhost:8080/echos" target="_blank">Here</a>.

## Add Request Handler
-Add newhandler.h and newhandler.cc in include and src folder and overwrite the function based on the need for the new 
handler
```
#ifndef NEW_HANDLER_H
#define NEW_HANDLER_H
#include "request_handler.h"
#include "request.h"
#include "header.h"
#include "config_parser.h"

namespace http {
namespace server {
class NEWHandler : public RequestHandler {
public:

	DefaultHandler();
	/// Construct with a directory containing files to be served.
	NEWHandler(std::map<std::string, 
	    				   http::server::handler_parameter>& dir_map);
	/**
	Create a echo handler according to the config file and root path.
	@param  NgixnConfig file and root path
	@return RequestHandler object that contains new handler
	*/
	static RequestHandler* create(const NginxConfig& config, 
								  const std::string& root_path);

	virtual bool handleRequest(const request& req, std::string& response) override;
	/**
	Given a request, this function will prepare a Response object 
	@param  request obejct
	@return unique pointer of Response
	*/
	virtual std::unique_ptr<http::server::Response> HandlerRequest(const request& request) override;
};
}
}

#endif
```