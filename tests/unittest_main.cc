#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <string>
#include <boost/bind.hpp>
#include <boost/asio.hpp>

boost::system::error_code ProcessRequest(tcp::socket* sock) {
    [...]
}

class MockSocket : public tcp::socket {
 public:
  MOCK_METHOD2(read_buf, size_t(asio::buffer&, asio::error_code&));
  MOCK_METHOD2(write_buf, size_t(asio::buffer&, asio::error_code&));
}

TEST_F(ProcessRequestTest, ReadRequest) {
    MockSocket socket;
    EXPECT_CALL(socket, read_some(_,_))
        .Times(AtLeast(1)).WillRepeatedly(Return(0));
    EXPECT_CALL(socket, write_some(_,_))
        .Times(AtLeast(1)).WillRepeatedly(Return(100));

    // false == no error
    EXPECT_FALSE(ProcessRequest(&socket));
}



int main(int argc, char* argv[]) {
    boost::asio::io_service io_service;
    tcp::acceptor a(*io_service, tcp::endpoint(
        tcp::v4(), 12345));
    while (true) {
        tcp::socket sock(*io_service);
        a.accept(sock);

        auto error = ProcessRequest(&sock);
        if (error) {
            printf("ProcessRequest() failed: %d: %s\n",
                   error.value(), error.message().c_str());
        }
    }
    return 0;
}