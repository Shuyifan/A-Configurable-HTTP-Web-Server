#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "utils.h"

class UtilsTest : public ::testing::Test {
  protected:
};

TEST_F(UtilsTest, WorkingDirTest) {
    std::string working_dir = get_server_dir();
    int length = working_dir.length();
    EXPECT_EQ(working_dir.substr(length - 17), "/cracking-the-web");
}