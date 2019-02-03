#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "mime_types.h"

class MimeTypeTest : public ::testing::Test {
  protected:
};

TEST_F(MimeTypeTest, KnownTypeTest) {
    std::string type = http::server::mime_types::extension_to_type("gif");
    EXPECT_EQ("image/gif", type);
    type = http::server::mime_types::extension_to_type("htm");
    EXPECT_EQ("text/html", type);
    type = http::server::mime_types::extension_to_type("html");
    EXPECT_EQ("text/html", type);
    type = http::server::mime_types::extension_to_type("jpg");
    EXPECT_EQ("image/jpeg", type);
    type = http::server::mime_types::extension_to_type("jpeg");
    EXPECT_EQ("image/jpeg", type);
    type = http::server::mime_types::extension_to_type("tar");
    EXPECT_EQ("application/x-tar", type);
    type = http::server::mime_types::extension_to_type("png");
    EXPECT_EQ("image/png", type);
    type = http::server::mime_types::extension_to_type("txt");
    EXPECT_EQ("text/plain", type);
    type = http::server::mime_types::extension_to_type("zip");
    EXPECT_EQ("application/zip", type);
}

TEST_F(MimeTypeTest, UnknownTypeTest) {
    std::string type = http::server::mime_types::extension_to_type("rar");
    EXPECT_EQ("text/plain", type);
    type = http::server::mime_types::extension_to_type(".cc");
    EXPECT_EQ("text/plain", type);
}