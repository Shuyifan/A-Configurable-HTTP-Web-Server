#include "gtest/gtest.h"
#include "config_parser.h"

TEST(NginxConfigParserTest, SimpleConfig) {
  NginxConfigParser parser;
  NginxConfig out_config;

  bool success = parser.Parse("example_config", &out_config);

  EXPECT_TRUE(success);
}

class ConfigParserTest : public ::testing::Test {
  protected:
    void SetUp() override {}

    NginxConfig out_config;
    NginxConfigParser parser;
};

TEST_F(ConfigParserTest, FileNotExist) {
  EXPECT_FALSE(parser.Parse("example", &out_config));
}

TEST_F(ConfigParserTest, ExtraSemicolon) {
  EXPECT_FALSE(parser.Parse("extra_semicolon", &out_config));
}

TEST_F(ConfigParserTest, ConsecutiveEndBlock) {
  EXPECT_TRUE(parser.Parse("consecutive_end_block", &out_config));
}

TEST_F(ConfigParserTest, RedundantEndBlock) {
  EXPECT_FALSE(parser.Parse("redundant_end_block", &out_config));
}

TEST_F(ConfigParserTest, RedundantStartBlock) {
  EXPECT_FALSE(parser.Parse("redundant_start_block", &out_config));
}

TEST_F(ConfigParserTest, CompleteConfig) {
  EXPECT_TRUE(parser.Parse("complete_config", &out_config));
}

TEST_F(ConfigParserTest, MissingSpace) {
  EXPECT_FALSE(parser.Parse("missing_space", &out_config));
}