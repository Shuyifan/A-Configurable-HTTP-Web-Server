#include <boost/asio.hpp>
#include <boost/log/trivial.hpp>
#include <sstream>

#include "proxy_handler.h"
#include "mime_types.h"

using boost::asio::ip::tcp;

namespace http {
namespace server {
    RequestHandler* ProxyHandler::create(const NginxConfig& config, 
								         const std::string& root_path) {
        ProxyHandler* handler = new ProxyHandler(config, root_path);
        return handler;
    }

    std::unique_ptr<Response> ProxyHandler::HandlerRequest(const request& request) {
        std::unique_ptr<Response> response_ (new Response);

        boost::asio::io_service io_service;

        tcp::resolver resolver(io_service);
        std::string host = getHost(handler_config_);
        std::string portNum = getPortNum(handler_config_);
        BOOST_LOG_TRIVIAL(info) << "Proxy host: " << host;
        BOOST_LOG_TRIVIAL(info) << "Host port: " << portNum;
        tcp::resolver::query query(host, portNum);
        tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

        tcp::socket socket(io_service);
        socket.connect(endpoint_iterator->endpoint());

        boost::asio::streambuf req;
        std::ostream request_stream(&req);
        std::string url = getDestURL(handler_config_, request.uri);
        request_stream << "GET " << url << " HTTP/1.1\r\n";
        request_stream << "Host: " << host << ":" << portNum << "\r\n";
        request_stream << "Accept: */*\r\n";
        request_stream << "Connection: close\r\n\r\n";

        boost::asio::write(socket, req);

        boost::asio::streambuf response;
        boost::asio::read_until(socket, response, "\r\n");

        std::istream response_stream(&response);
        std::string http_version;
        response_stream >> http_version;
        unsigned int status_code;
        response_stream >> status_code;
        std::string status_message;
        std::getline(response_stream, status_message);
        if(!response_stream || http_version.substr(0, 5) != "HTTP/") {
            BOOST_LOG_TRIVIAL(warning) << "Invalid Response";
        }
        if(status_code == 302) {
            BOOST_LOG_TRIVIAL(info) << "Redirect";
        }

        boost::asio::read_until(socket, response, "\r\n\r\n");

        std::string header;
        while(std::getline(response_stream, header) && header != "\r") {
            BOOST_LOG_TRIVIAL(info) << "Response header: " << header;
        }

        std::string content;
        std::stringstream contentss;
        if(response.size() > 0) {
            std::getline(response_stream, content);
            contentss << content;
        }

        boost::system::error_code error;
        while(boost::asio::read(socket, response,
            boost::asio::transfer_at_least(1), error)) {
            contentss << &response;
        }
        if(error != boost::asio::error::eof) {
            BOOST_LOG_TRIVIAL(error) << "Error!";
            response_->SetStatus(Response::service_unavailable);
            response_->SetContent("Read error!");
        } else {
            response_->SetStatus(Response::ok);
            response_->SetContent(contentss.str());
            response_->AddHeader("Content-Length", std::to_string(contentss.str().size()));
            response_->AddHeader("Content-type", mime_types::extension_to_type("html"));
        }
        return response_;
    }

    std::string ProxyHandler::getHost(NginxConfig& config) {
        for(const auto& statement : config.statements_) {
            if(statement->tokens_[0] == "host") {
                return statement->tokens_[1];
            }
        }
        return "";
    }

    std::string ProxyHandler::getPortNum(NginxConfig& config) {
        for(const auto& statement : config.statements_) {
            if(statement->tokens_[0] == "port") {
                return statement->tokens_[1];
            }
        }
        return "80";
    }

    std::string ProxyHandler::getDestURL(NginxConfig& config, std::string requestURL) {
        std::string location = "";
        for(const auto& statement : config.statements_) {
            if(statement->tokens_[0] == "location") {
                location = statement->tokens_[1];
            }
        }
        if(!location.empty()) {
            if(requestURL.length() == location.length()) {
                BOOST_LOG_TRIVIAL(info) << "Dest URL: " << "/";
                return "/";
            } else {
                BOOST_LOG_TRIVIAL(info) << "Dest URL: " << requestURL.substr(location.length());
                return requestURL.substr(location.length());
            }
        }
        BOOST_LOG_TRIVIAL(error) << "Error in getDestURL";
        return "";
    }

} // namespace server
} // namespace http