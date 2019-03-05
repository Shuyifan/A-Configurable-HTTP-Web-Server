#include "gtest/gtest.h"
#include "proxy_handler.h"

#include "config_parser.h"
#include "request.h"
#include "request_parser.h"
#include "response.h"
#include "response_parser.h"
#include <boost/log/trivial.hpp>

#include <sstream>
#include <string>

using namespace std;

namespace http {
namespace server {

// Dummy class that effectively mocks Client
class DummyClient: public Client {
public:
    DummyClient(std::string remoteAddress, int outgoingPort) {
        address_ = remoteAddress;
        port_ = outgoingPort;
    }
    std::unique_ptr<http::server::Response> getResponse(request& req) override {
        std::unique_ptr<http::server::Response> resp(new Response());
        if( address_ == "redirect2" ) {
            resp->SetStatus(Response::moved_temporarily);
            resp->AddHeader("Location", "redirect1/from2"+req.uri);
        } else if ( address_ == "redirect1" ) {
            resp->SetStatus(Response::moved_temporarily);
            resp->AddHeader("Location", "noredirect/from1"+req.uri);
        } else {
            resp->SetStatus(Response::ok);
        }
        
        resp->AddHeader("RequestedUrl", req.uri);
        resp->AddHeader("Address", address_);
        resp->AddHeader("OutgoingPort", std::to_string(port_));
        resp->SetContent(req.body);
        return resp;
    }
};

// Dummy class that (1) uses the dummy (effectively mocked) version of Client
// and (2) exposes protected methods for testing
class DummyProxyHandler: public ProxyHandler {
public:
    static void split(const std::string& combinedAddress, std::string& addressOut, std::string& locationOut, size_t startPosition=0) {
        splitCombinedAddress(combinedAddress, addressOut, locationOut, startPosition);
    }

    std::unique_ptr<Client> createClient(std::string remoteAddress, int outgoingPort) override {
        std::unique_ptr<Client> client(new DummyClient(remoteAddress, outgoingPort));
        return client;
    }

    static DummyProxyHandler* create(const NginxConfig& config, const std::string& root_path)
    {
        BOOST_LOG_TRIVIAL(info) << "Creating proxy handler";
        http::server::DummyProxyHandler* handler = new http::server::DummyProxyHandler();
        readConfig(config, handler);
        BOOST_LOG_TRIVIAL(trace) << "Returning created proxy handler";
        return handler;
    }
};

static request parseRequest(string requestString) {
    request req;
    request_parser parser;
    parser.parse(req, requestString.begin(), requestString.end());
    return req;
}
static Response parseResponse(string respString) {
    Response resp;
    ResponseParser parser;
    parser.parse(resp, respString);
    return resp;
}

static NginxConfig parseConfig(string configString) {
    stringstream ss;
    ss << configString;
    NginxConfigParser parser;
    NginxConfig config;
    bool success = parser.Parse(&ss, &config);
    EXPECT_TRUE(success);
    return config;
}


TEST(ProxyHandlerTest, SimpleDummyProxy) {
    string requestString = "GET /proxylocation/test/test2 HTTP/1.1\r\n\r\n";
    request req = parseRequest(requestString);

    string configString = "location /proxylocation;\naddress www.proxied.server.com;\nport 12347;";
    NginxConfig config = parseConfig(configString);
    DummyProxyHandler* handler = DummyProxyHandler::create(config, "dummyRoot");
    unique_ptr<Response> responseActual = handler->HandlerRequest(req);

    EXPECT_EQ(responseActual->getHeader("RequestedUrl"), "/test/test2");
    EXPECT_EQ(responseActual->getHeader("Address"), "www.proxied.server.com");
    EXPECT_EQ(responseActual->getHeader("OutgoingPort"), "12347");
}

TEST(ProxyHandlerTest, DummyProxyWithPrefix) {
    string requestString = "GET /proxylocation/test/test2 HTTP/1.1\r\n\r\n";
    request req = parseRequest(requestString);

    string configString = "location /proxylocation;\naddress www.proxied.server.com;\nport 12347;\nprefix the/prefix;";
    NginxConfig config = parseConfig(configString);
    DummyProxyHandler* handler = DummyProxyHandler::create(config, "dummyRoot");
    unique_ptr<Response> responseActual = handler->HandlerRequest(req);

    EXPECT_EQ(responseActual->getHeader("RequestedUrl"), "/the/prefix/test/test2");
    EXPECT_EQ(responseActual->getHeader("Address"), "www.proxied.server.com");
    EXPECT_EQ(responseActual->getHeader("OutgoingPort"), "12347");
}

TEST(ProxyHandlerTest, DummyProxyRedirect) {
    string requestString = "GET /proxylocation/test/test2 HTTP/1.1\r\n\r\n";
    request req = parseRequest(requestString);

    string configString = "location /proxylocation;\naddress redirect2;\nport 12347;";
    NginxConfig config = parseConfig(configString);
    DummyProxyHandler* handler = DummyProxyHandler::create(config, "dummyRoot");
    unique_ptr<Response> responseActual = handler->HandlerRequest(req);

    EXPECT_EQ(responseActual->getHeader("RequestedUrl"), "/from1/from2/test/test2");
    EXPECT_EQ(responseActual->getHeader("Address"), "noredirect");
    EXPECT_EQ(responseActual->getHeader("OutgoingPort"), "12347");
}


TEST(ProxyHandlerTest, ProxyCreateHandler) {
    // Just test that handler creation is successful, 
    //      neither returning null nor generating a runtime error
    string configString = "location /proxylocation;\naddress www.proxied.server.com;\nport 12347;\nprefix the/prefix;";
    NginxConfig config = parseConfig(configString);
    ProxyHandler* handler = ProxyHandler::create(config, "dummyRoot");
    ASSERT_NE(handler, nullptr);
}

TEST(ProxyHandlerTest, IncompleteConfig) {
    vector<string> invalidConfigStrings = {
        "address www.proxied.server.com;\nport 12347;",
        "location /proxylocation;\nport 12347;",
        "location /proxylocation;\naddress www.proxied.server.com;"
    };
    for (const string& configString: invalidConfigStrings) {
        NginxConfig config = parseConfig(configString);
        ProxyHandler* handler = ProxyHandler::create(config, "dummyRoot");
        EXPECT_EQ(handler, nullptr);
    }
}

TEST(ProxyHandlerTest, InvalidPorts) {
    vector<string> invalidPorts = {"-99", "-1", "0", "1.5", "1c0", "x"};
    for (const string& port: invalidPorts) {
        string configString = "location /proxylocation;\naddress www.proxied.server.com;\nport " + port + ";\nprefix the/prefix;";
        ProxyHandler* handler = ProxyHandler::create(parseConfig(configString), "dummyRoot");
        EXPECT_EQ(handler, nullptr);
    }
}


class ProxyHandlerHelperCombinedAddressTest: public ::testing::Test {
public:
    string combinedAddress;
    string addressOut;
    string locationOut;

    void doSplit() {
        DummyProxyHandler::split(combinedAddress, addressOut, locationOut);
    }

    void testSplit(string address, string location) {
        combinedAddress = address + location;
        doSplit();
        EXPECT_EQ(addressOut, address);
        if (location != "") 
            EXPECT_EQ(locationOut, location);
        else
            EXPECT_EQ(locationOut, "/");
    }
};

TEST_F(ProxyHandlerHelperCombinedAddressTest, TypicalCasesSplit) {
    testSplit("ucla.edu", "/something");
    testSplit("www.ucla.edu", "/something/other");
    testSplit("http://www.ucla.edu", "/");
    testSplit("test.blah.etc", "/nothing");
    testSplit("localhost", "/");
    testSplit("localhost:8080", "/");
}

TEST_F(ProxyHandlerHelperCombinedAddressTest, CornerCasesSplit) {
    string temp;
    // Corner cases

    temp = "addressInput";
    addressOut = temp;
    combinedAddress="";
    doSplit();
    EXPECT_EQ(addressOut, temp);
    EXPECT_EQ(locationOut, "/");

    combinedAddress = "aStringWithoutASlash";
    doSplit();
    EXPECT_EQ(addressOut, combinedAddress);
    EXPECT_EQ(locationOut, "/");

    temp = "aStringWithASlashAtTheEnd";
    combinedAddress = temp + "/";
    doSplit();
    EXPECT_EQ(addressOut, temp);
    EXPECT_EQ(locationOut, "/");

    // Note -- cases with doubled slashes are in a separate unit test
    
    // '/' in first character: entire string is location; leave addressOut unchanged
    temp = "addressInput";
    addressOut = temp;
    combinedAddress = "/aStringWithASlashAtTheStart";
    doSplit();
    EXPECT_EQ(addressOut, temp);
    EXPECT_EQ(locationOut, combinedAddress);
}

TEST_F(ProxyHandlerHelperCombinedAddressTest, EscapedSplit) {

    string tempPre, tempPost;
    
    testSplit("http://www.ucla.edu", "/some/subdirectory");
    testSplit("escaped//slashesOccurring//twice", "/some/subdirectory");
    testSplit("//escpaedSlashesAtTheBeginningAndAlsoOccurring//twice", "/some/subdirectory");
    // shouldn't occur in real life, but valid cases to test
    testSplit("http://someHost.blah", "/withEscapedSlashes//inTheLocation");
    testSplit("triple//", "/slashes");
    testSplit("quadruple////slashes", "/location");

    testSplit("onlySlashesAreDoubledAtTheEnd//", "");

}

}
}
