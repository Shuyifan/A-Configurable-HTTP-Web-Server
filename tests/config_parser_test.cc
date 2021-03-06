#include "gtest/gtest.h"
#include "config_parser.h"

TEST(NginxConfigParserTest, SimpleConfig) {
  NginxConfigParser parser;
  NginxConfig out_config;

  bool success = parser.Parse("config_test/example_config", &out_config);

  EXPECT_TRUE(success);
}

class ConfigParserTest : public ::testing::Test {
  protected:
    void SetUp() override {}

    NginxConfig out_config;
    NginxConfigParser parser;
};

TEST_F(ConfigParserTest, NginxConfigToString) {
  parser.Parse("config_test/example_config", &out_config);
  std::string str1 = "foo \"bar\";\nserver {\n  listen 80;\n  server_name foo.com;\n  root /home/ubuntu/sites/foo/;\n}\n";
  EXPECT_TRUE(str1 == out_config.ToString(0));
}

TEST_F(ConfigParserTest, FileNotExist) {
  EXPECT_FALSE(parser.Parse("config_test/example", &out_config));
}

TEST_F(ConfigParserTest, ExtraSemicolon) {
  EXPECT_FALSE(parser.Parse("config_test/extra_semicolon", &out_config));
}

TEST_F(ConfigParserTest, ConsecutiveEndBlock) {
  EXPECT_TRUE(parser.Parse("config_test/consecutive_end_block", &out_config));
}

TEST_F(ConfigParserTest, RedundantEndBlock) {
  EXPECT_FALSE(parser.Parse("config_test/redundant_end_block", &out_config));
}

TEST_F(ConfigParserTest, RedundantStartBlock) {
  EXPECT_FALSE(parser.Parse("config_test/redundant_start_block", &out_config));
}

TEST_F(ConfigParserTest, CompleteConfig) {
  EXPECT_TRUE(parser.Parse("config_test/complete_config", &out_config));
}

TEST_F(ConfigParserTest, MissingSpace) {
  EXPECT_FALSE(parser.Parse("config_test/missing_space", &out_config));
}