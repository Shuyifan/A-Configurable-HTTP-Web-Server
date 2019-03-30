# A-Configurable-HTTP-Web-Server
Our poject is a configurable webserver which can handle echo, static file,and other ruquest in c++.
## Contributor
Chengshun Zhang<br> 
Yifan Shu<br> 
Yuqing Wang<br> 
Nelly Lyu<br> 
## Project Structure
#### A typical top-level directory layout
    .   
    ├── cmake                           # CMake config file for generating coverage report
    ├── conf                            # deploy.conf file is inside
    ├── docker                          # Docker information
    ├── environment                     # The environment for the project
    ├── files                           # Contains static files inside the folder
    ├── include                         # Contains all the header files
    ├── src                             # Contains all source files
    ├── tests                           # Contains unit tests and integration test
    ├── README.md                       # Readme file
    └── CMakeList.txt                   # CMake config file
    └── CMakeList.txt                   # CMake config file
    
#### Information about header file
    .
    ├── ...
    ├── include                         # Contains all the header files
    │   ├── handler                     # Contains all the handler's header files
    │   │   ├── accept_handler.h        # A handler to deal with actually creating the meme, and storing them into locals
    │   │   ├── bad_request_handler.h   # A handler to deal with error request
    │   │   ├── create_form_handler.h   # A handler to deal with creating a meme
    │   │   ├── default_handler.h       # Handle the request to main page
    │   │   ├── delete_handler.h        # A handler deals with deleting a spcific generated meme
    │   │   ├── echo_handler.h          # Handle the echo request
    │   │   ├── error_handler.h         # A handler to deal with error request
    │   │   ├── health_handler.h        # A handler to detect whether the server is working properly
    │   │   ├── list_meme_handler.h     # A handler deals with listing all the generated memes
    │   │   ├── proxy_handler.h         # A handler to deal with reverse proxy
    │   │   ├── request_handler.h       # Interface class of all the handler
    │   │   ├── search_handler.h        # A handler deals with searching the generated memes
    │   │   ├── static_handler.h        # A handler deals with serving the static files
    │   │   ├── status_handler.h        # A handler deals with providing the status information about the server
    │   │   └── view_meme_handler.h     # A handler deals with view the generated meme
    │   ├── config_parser.h             # Take a opened config file or file name and store the parsed config in the provided NginxConfig out-param
    │   ├── handler_manager.h           # A manager class to server correct request handler
    │   ├── handler_parameter.h         # Contains directory and log as well as handler name and config
    │   ├── header.h                    # Headers object for the HTTP protocols
    │   ├── mime_types.h                # Convert a file extension into a MIME type.
    │   ├── request_parser.h            # Parse the request
    │   ├── request.h                   # Request object
    │   ├── response.h                  # Response object
    │   ├── server.h                    # For server
    │   ├── session.h                   # For session
    │   └── utils.h                     # Contains all the utility functions
    └── ...
## Getting Started
#### Setting up the development environment
The requirement for the development environment for this project can be shown as follows:
* Boost C++ Libraries
* Clang
* CMake
* Docker
* Google Cloud SDK
* Google Test
* Git 
* git-review
* LLDB 

To install all the package, we use docker to configuarate our development. [Docker](https://www.docker.com/) should be installed first. The installation tutorial can be seen here: https://docs.docker.com/install/.<br> 
After installing the docker, the environment can be simply set up through the following command line:
```
environment/env/start.sh -u ${USER} -n
```
#### Clone the repository
```
git clone https://github.com/Shuyifan/A-Configurable-HTTP-Web-Server.git
```
#### Build locally
First change the conf/deploy.conf listen 80 to listen 8080
```
cd build
cmake ..
make 
make test
```
#### Docker Build
A code coverage build of a binary includes extra code to generate output files showing which lines and branches were executed. To generate code coverage reports for the CMakeLists.txt generated from our project templates:
```
environment/env/start.sh -u ${username}
cd cracking-the-web
./temp.sh 
docker run --rm -p 127.0.0.1:8080:8080 --name my_run my_image:latest
```
This will create an HTML report in ${REPO}/build_coverage/report/index.html that you can view in your browser.
#### Testing and get the coverage report
```
mkdir build_coverage
cd build_coverage
cmake -DCMAKE_BUILD_TYPE=Coverage ..
make coverage
```
## Add Request Handler
To add a new different type of request handler, several things should be done as follows:

#### 1. Inherit from the request_handler class:
For this project, every handler class implement the class request_handler, which can be found in /include/request_handler.h. The defination is shown as follows:
```
class RequestHandler {
public:
	RequestHandler() {};
	
	/**
	Handle a parsed HTTP request, and generates a response
	@param  request object
	@return response object
	*/
	//virtual bool handleRequest(const request& req, std::string& response) = 0;

	virtual std::unique_ptr<http::server::Response> HandlerRequest(const request& request) = 0;

};
```
The HandlerRequest function should be implemented. The HandlerRequest function take an input request (detail information can be see in /include/request.h) and return a output response (detail information can be see in /include/response.h).

#### 2. Create a static instantiation method for a specifc handler class
After implement the HandlerRequest function, one static instantiation method should be declared and implemented in the handler. The defination should follow the following rules:
```
class newHandler : public RequestHandler {
public:

	static RequestHandler* create(const NginxConfig& config, 
								  const std::string& root_path);

	virtual std::unique_ptr<http::server::Response> HandlerRequest(const request& request) override;
};
```
The argument "NginxConfig& config" include all the information during the handler instantiation and the argument "const std::string& root_path" specify the working directory for this handler.
#### 3. Register the new handler class in the HandlerManager
After implmenting the new handler, you have to register this class in the HandlerManger. You should add your new handler in the HandlerManager::createByName function. following the following format:
```
std::unique_ptr<http::server::RequestHandler> HandlerManager::createByName(const std::string& name, 
									 					                   const NginxConfig& config, 
                                     					                   const std::string& root_path) {
    std::unique_ptr<http::server::RequestHandler> handler;
    if(name == "static") {
        handler.reset(http::server::StaticHandler::create(config, root_path));
        BOOST_LOG_TRIVIAL(info) << "Request for static file server";
    } else if(name == "echo") {
        handler.reset(http::server::EchoHandler::create(config, root_path));
        BOOST_LOG_TRIVIAL(info) << "Request for echo server";
    } else if(name == "error") {
        handler.reset(http::server::ErrorHandler::create(config, root_path));
        BOOST_LOG_TRIVIAL(info) << "Error Handler called";
    } else if(name == "status") {
        handler.reset(http::server::StatusHandler::create(config, root_path));
        BOOST_LOG_TRIVIAL(info) << "Status Handler called";
    } else {
        handler.reset(http::server::DefaultHandler::create(config, root_path));
    }
    return handler;
} 
```
For example, if our new handler's name is "new" and its corresponding class is called NewHandler. Then the code after the revision will look as follows:
```
std::unique_ptr<http::server::RequestHandler> HandlerManager::createByName(const std::string& name, 
									 					                   const NginxConfig& config, 
                                     					                   const std::string& root_path) {
    std::unique_ptr<http::server::RequestHandler> handler;
    if(name == "static") {
        handler.reset(http::server::StaticHandler::create(config, root_path));
        BOOST_LOG_TRIVIAL(info) << "Request for static file server";
    } else if(name == "echo") {
        handler.reset(http::server::EchoHandler::create(config, root_path));
        BOOST_LOG_TRIVIAL(info) << "Request for echo server";
    } else if(name == "error") {
        handler.reset(http::server::ErrorHandler::create(config, root_path));
        BOOST_LOG_TRIVIAL(info) << "Error Handler called";
    } else if(name == "status") {
        handler.reset(http::server::StatusHandler::create(config, root_path));
        BOOST_LOG_TRIVIAL(info) << "Status Handler called";
    } else if(name == "new") {
/**-------------------------------------------------------------------------**/
        handler.reset(http::server::NewHandler::create(config, root_path));
        BOOST_LOG_TRIVIAL(info) << "The New Handler called";
**-------------------------------------------------------------------------**/
    } else {
        handler.reset(http::server::DefaultHandler::create(config, root_path));
    }
    return handler;
} 
```
After registering the handler in the HandlerManager, the new handler can work properly after we set the config file.
## Configurate the Server
For this project, the server is configuarble so that we can use customize config file to deploy our server.

Here is a sample config file which demonstrate the basic syntax of the config:
```
listen 80;

root /files;

handler static {
    location /static;
    root static;
}

handler static {
    location /foo;
    root static;
}

handler echo {
    location /echo;
}

handler error {
    location /;
}

handler status {
    location /status;
}
```
#### "Listen" Keyword
In this example, we set "listen 80", which indicates that our server will listen the port 80.
#### "root" Keyword in the Outter Block
The "root" keyword specify the root directory of all the handler work directory. Suppose our project is located in "c/user/cracking-the-web", then in this particular example, all the handlers' is based on the directory "c/user/cracking-the-web/files".
#### "handler" Keyword
This keyword specify the name of the handler and its corresponding config. Take the following as an example：
```
handler static {
    location /static;
    root static;
}
```
It specifies a static handler, which is defined in "/include/HandlerManager.cc", where static handler will use the name to instantiate a StaticHandler object.
After the name, the rest of the statement:
```
{
    location /static;
    root static;
}
```
is the config of this handler.
#### "location" Keyword in the Inner Block
This will specify when a handler will be used given a specifc URL. Take the following as an example:
```
handler static {
    location /static;
    root static;
}
```
This config specify that when a request's URI match with /static, a static handler will be used to handle this request.
For this matches, we will use a "longest directory matching metrics".

Taking the following config:
```
handler static {
    location /foo/file1;
}
handler echo {
    location /foo;
}
handler error {
    location /;
}
```
**Example 1:**
If a request with URL "/foo/file1/tt/index.html" is comming, it will first check "/foo/file1/tt/index.html". Because it cannot find any matched one, so it will then check "/foo/file1/tt", but sitill cannot find a matched one. Then it will checked "/foo/file1", which is matched with:
```
handler static {
    location /foo/file1;
}
```
In this case, "/foo/file1" is the longest matched directory, and thus this request will be handler by this handler.
**Example 2:**
If a request with URL "/foo" is comming, it will checked "/foo", which is matched with:
```
handler echo {
    location /foo;
}
```
In this case, "/foo" is the longest matched directory, and thus this request will be handler by this handler.
**Example 3:**
If a request with URL "/unkonwn/index.html" is comming, it will first check "/unkonwn/index.html". Because it cannot find any matched one, so it will then check "/unkonwn", but sitill cannot find a matched one. Then it will checked "/"
```
handler error {
    location /;
}
```
In this case, "/" is the longest matched directory, and thus this request will be handler by this handler.
#### "root" Keyword in the Inner Block
The "root" keyword specify the root directory of a specific handler work directory. Suppose our project is located in "c/user/cracking-the-web" and the config is shown as follows:
```
root /files;

handler static {
    location /static;
    root static;
}
```
The directory "c/user/cracking-the-web/files" will be the base for all the handler. And the "c/user/cracking-the-web/files/static" will be the root directory for this specific static handler.
#### Notice about the Inner Block:
The inner block is a config for a specifc handler. Basiclly, it can have whatever keyword as long as the keyword is handled properly by that handler.