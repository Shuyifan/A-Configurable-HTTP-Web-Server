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
}

TEST_F(UtilsTest, FileNameTest) {
    std::string input = "/files/static/index.html";
    std::string file_name = get_file_name(input);
    EXPECT_EQ(file_name, "/index.html");
}