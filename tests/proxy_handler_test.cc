#include "gtest/gtest.h"
#include "proxy_handler.h"

using namespace std;

namespace http {
namespace server {

// Dummy class to expose protected methods for testing
class DummyProxyHandler: public ProxyHandler {
public:
    static void split(const std::string& combinedAddress, std::string& addressOut, std::string& locationOut, size_t startPosition=0) {
        splitCombinedAddress(combinedAddress, addressOut, locationOut, startPosition);
    }
};

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
