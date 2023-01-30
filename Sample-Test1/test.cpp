#include "pch.h"
using namespace noveil;
TEST(TestCaseName, TestName) {
  nlet<std::string> v1("hello");
  std::vector<nlet<std::string>> vec1;
  vec1.emplace_back(std::string("fffd"));
  vec1.emplace_back(std::move(v1));

  EXPECT_EQ(std::string("hello"), *vec1.back());
}
