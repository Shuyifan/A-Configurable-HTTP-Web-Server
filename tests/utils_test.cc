#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "utils.h"

class UtilsTest : public ::testing::Test {
  protected:
};

TEST_F(UtilsTest, WorkingDirTest) {
    std::string working_dir = get_server_dir();
    EXPECT_TRUE(!working_dir.empty());
}

TEST_F(UtilsTest, UpperDirTest) {
    std::string input = "/files/static/index.html";
    std::string upper_dir = get_upper_dir(input);
    EXPECT_EQ(upper_dir, "/files/static");
    input = "";
    upper_dir = get_upper_dir(input);
    EXPECT_EQ(upper_dir, "");
    input = "index.html";
    upper_dir = get_upper_dir(input);
    EXPECT_EQ(upper_dir, "");
}

TEST_F(UtilsTest, FileNameTest) {
    std::string input = "/files/static/index.html";
    std::string file_name = get_file_name(input);
    EXPECT_EQ(file_name, "/index.html");
    input = "index.html";
    file_name = get_file_name(input);
    EXPECT_EQ(file_name, "");
}

TEST_F(UtilsTest, UrlDecodeTest) {
    std::string url = "/files/static/index%20world+1.html";
    std::string out = "";
    EXPECT_TRUE(url_decode(url, out));
    EXPECT_EQ(out, "/files/static/index world 1.html");
}