#include "pch.h"
using namespace noveil;

class TestParser : public NFileParserTemplate<std::string, std::string> {};

TEST(FuncMapTest, Simple) {
  // 関数オブジェクトを呼び出せるかどうかテスト
  TestParser parser;

  struct A {
    std::string operator()() { return std::move(std::string("This is Test.")); };
  };
  A a;

  TestParser::AddFunctor("test", a);
  EXPECT_EQ("This is Test.", parser.CallFunctor("test"));
}
