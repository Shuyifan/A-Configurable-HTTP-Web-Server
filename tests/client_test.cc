#include "gtest/gtest.h"
#include "client.h"

using namespace std;

namespace http {
namespace server {

// Dummy class to expose protected methods for testing
class DummyClient: public Client {
public:
    static bool split(const std::string& address, std::string& hostnameOut, std::string& portOut, size_t startPos=0) {
        splitAddress(address, hostnameOut, portOut, startPos);
    }
};

class ClientHelperSplitAddressTest: public ::testing::Test {
public:
    string addressIn;
    string hostnameOut;
    string portOut;
    bool successOut;

    void doTest(string hostExp, string portExp) {
        successOut = DummyClient::split(addressIn, hostnameOut, portOut);
        EXPECT_EQ(hostnameOut, hostExp);
        EXPECT_EQ(portOut, portExp);
    }
    void autoTest(string host, string port) {
        addressIn = host + ":" + port;
        doTest(host, port);
    }
};

TEST_F(ClientHelperSplitAddressTest, Typical) {
    addressIn = "localhost";
    doTest("localhost", "80");

    autoTest("localhost", "8080");

    addressIn = "http://localhost:8080";
    doTest("localhost", "8080");

    addressIn = "http://www.ucla.edu:8080";
    doTest("www.ucla.edu", "8080");

    addressIn = "http://www.ucla.edu";
    doTest("www.ucla.edu", "80");
}

}
}

