#pragma once

#include <string>
#include <vector>
#include <map>
#include "header.h"


namespace http {
namespace server {
class Response {
public:
    //this includes the status code,setter and getter function of response
    enum StatusCode {
        ok = 200,
        created = 201,
        accepted = 202,
        no_content = 204,
        multiple_choices = 300,
        moved_permanently = 301,
        moved_temporarily = 302,
        not_modified = 304,
        bad_request = 400,
        unauthorized = 401,
        forbidden = 403,
        not_found = 404,
        internal_server_error = 500,
        not_implemented = 501,
        bad_gateway = 502,
        service_unavailable = 503
    };

    Response() {
        version_ = "1.1";
    }

    StatusCode getStatusCode(int status_code);

    void SetStatus(const StatusCode status_code) {
        status_code_ = status_code;
    }

    StatusCode GetStatus(){
        return status_code_;
    }

    std::string GetVersion(){
        return version_;
    }

    void AddHeader(const std::string& name, const std::string& value) {
        headers[name] = value;
    }
    void SetContent(const std::string& body) {
        content_ = body;
    }

    void SetVersion(const std::string& version) {
        version_ = version;
    }

    void SetIntStatus(int code) {
        status_code_int_ = code; // temporary for now before lookup table is added
    }

    StatusCode StatusLookup(int code);

    std::string ToString();

    std::string getStatusLine();

    bool headerExists(std::string headerKey);
    std::string getHeader(std::string headerKey);
    StatusCode getStatus() const;

    private:
        std::string mime_type_;
        std::map<std::string, std::string> headers;
        std::string content_;
        StatusCode status_code_;
        std::string resString(StatusCode status);
        std::string version_;

        // alternate versions of variables for ProxyHandler
        std::vector<header> header_vec;
        int status_code_int_;

};
} // namespace server
} // namespace http
