#include <sstream>
#include "gtest/gtest.h"
#include "handler_manager.h"

class ListMemeHandlerTest : public ::testing::Test {
protected:

    void SetUp() {
        request.method = "GET";
        request.uri = "/meme/list";
        request.http_version_major = 1;
        request.http_version_minor = 1;
        parser.Parse("test.conf", &config);
        manager = new http::server::HandlerManager(config);
        handler = manager->createByUrl("/meme/list");
        response = handler->HandlerRequest(request);
        resStr = response->ToString();
    }

    std::unique_ptr<http::server::RequestHandler> handler;
    NginxConfig config;
    NginxConfigParser parser;
    http::server::HandlerManager* manager;
    http::server::request request;
    std::unique_ptr<http::server::Response> response;
    std::string resStr;
};

TEST_F(ListMemeHandlerTest, ResponseOK) {
    EXPECT_EQ(resStr.substr(0, 15), "HTTP/1.1 200 OK");
}

TEST_F(ListMemeHandlerTest, responseTest) {
    std::stringstream ss;
    ss << "<!DOCTYPE html>";
    ss << "<html lang=\"en\" dir=\"ltr\">";
    ss << "<head>";
    ss << "<meta charset=\"utf-8\">";
    ss << "<title>Meme List</title>";
    ss << "<link rel=\"stylesheet\" href=\"https://stackpath.bootstrapcdn.com/bootstrap/4.3.1/css/bootstrap.min.css\" integrity=\"sha384-ggOyR0iXCbMQv3Xipma34MD+dH/1fQ784/j6cY/iJTQUOhcWr7x9JvoRxT2MZw1T\" crossorigin=\"anonymous\">";
    ss << "</head>";
    ss << "<body>";
    ss << "<div class=\"container\">";
    ss << "<h2>The Meme List</h2>";
    ss << "<table class=\"table table-striped\">";
    ss << "<thead>";
    ss << "<tr>";
    ss << "<th scope=\"col\">ID</th>";
    ss << "<th scope=\"col\">Image </th>";
    ss << "<th scope=\"col\">Top Text</th>";
    ss << "<th scope=\"col\">Bottom Text</th>";
    ss << "</tr>";
    ss << "<tbody>";
    ss << "<tr><th scope=\"row\">1</th><td>wings.jpg</td><td>id1_test</td><td>id1_test</td></tr>";
    ss << "<tr><th scope=\"row\">2</th><td>psyduck.jpg</td><td>id2_test</td><td>id2_test</td></tr>";
    ss << "<tr><th scope=\"row\">3</th><td>psyduck.jpg</td><td>id3_test</td><td>id3_test</td></tr>";
    ss << "</tbody>";
    ss << "</thead>";
    ss << "</table>";
    ss << "</div>";
    ss << "</body>";
    ss << "</html>";
    
    // EXPECT_EQ(response->getBody(), ss.str());
}