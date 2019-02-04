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