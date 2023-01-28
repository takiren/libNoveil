#include "pch.h"
using namespace noveil;

class TestParser : public NFileParserTemplate<std::string, std::string> {};
class StrStrStrParser
    : public NFileParserTemplate<std::string, std::string, std::string> {};

TEST(FuncMapTest, Simple) {
  // 関数オブジェクトを呼び出せるかどうかテスト

  struct A {
    std::string operator()() {
      return std::move(std::string("This is Test."));
    };
  };
  A a;
  TestParser::AddFunctor("test", a);
  EXPECT_EQ("This is Test.", TestParser::CallFunctor("test"));
}

TEST(FuncMapTest, Forwarding) {
  struct Str {
    std::string operator()(std::string ss) { return ss; }
  };
  {
    Str tst;
    StrStrStrParser::AddFunctor("test2", tst);
  }

  EXPECT_EQ("Forwarding", StrStrStrParser::CallFunctor("test2", "Forwarding"));
}

class Parser3 : public NFileParserTemplate<int, std::string, int, int> {};

TEST(FuncMapTest, Forwarding2) {
  struct multiply {
    int operator()(int x, int y) { return x * y; }
  };

  Parser3::AddFunctor("test1", [](int x, int y) { return x * y; });

  EXPECT_EQ(30, Parser3::CallFunctor("test1", 5, 6));
}

TEST(NNodeTest, BindCopy) {
  auto* node = new NNodeBase();
  auto* nin = new NNodePinInput();
  {
    struct A {
      std::string operator()() { return "This is A."; };
    };

    A a;
    nin->Bind(a);
    auto str = nin->GetData();
  }
  auto* nout = new NNodePinOutput();
  {
    struct B {
      std::string operator()() { return "This is B."; };
    };

    B b;
    nout->Bind(b);
  }
  EXPECT_EQ("This is A.", nin->GetData().Get<std::string>());
  EXPECT_EQ("This is B.", nout->GetData().Get<std::string>());

  auto instance = std::make_shared<NNodeExecutionTest1>();
  {
    node->AddInputNode(nin);
    node->AddOutputNode(nout);
    node->AddInputNode(new NNodePinInput());
  }

  EXPECT_EQ(2, node->GetPinInputRef().size());
  EXPECT_EQ(1, node->GetPinOutputRef().size());
}