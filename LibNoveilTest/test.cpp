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
  auto* nin = new NNodePinBase();
  {
    struct A {
      std::string operator()() { return "This is A."; };
    };

    A a;
    nin->Bind(a);
    auto str = nin->value();
  }
  auto* nout = new NNodePinBase();
  {
    struct B {
      std::string operator()() { return "This is B."; };
    };

    B b;
    nout->Bind(b);
  }
  EXPECT_EQ("This is A.", nin->value().value().Get<std::string>());
  EXPECT_EQ("This is B.", nout->value().value().Get<std::string>());

  {
    node->AddInputNode(nin);
    node->AddOutputNode(nout);
    node->AddInputNode(new NNodePinBase());
  }

  EXPECT_EQ(2, node->GetPinInputRef().size());
  EXPECT_EQ(1, node->GetPinOutputRef().size());
}

TEST(SequencerTest, case1) {
  auto* seq = new NSequencer();
  auto node1 = seq->AddNode();
  auto node2 = seq->AddNode();
  node1.lock()->BindToNextNode(node2);
  node1.lock()->SetImplementation(
      [](PackedParentRef<NNodePinBase>, PackedParentRef<NNodePinBase>) {
        std::cout << "node1" << std::endl;
        _sleep(2000);
        return;
      });
  node2.lock()->SetImplementation(
      [](PackedParentRef<NNodePinBase>, PackedParentRef<NNodePinBase>) {
        std::cout << "node2" << std::endl;
        return;
      });

  std::thread th([&]() {
    seq->Execute();
    return;
  });
  std::cout << "FInished." << std::endl;
  _sleep(4000);
}
