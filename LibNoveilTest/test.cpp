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
  auto nin = node->AddInputNode();
  {
    struct A {
      std::string operator()() { return "This is A."; };
    };

    A a;
    if (auto r = nin.borrow()) {
      r->Bind(a);
      auto str = r->value();
    }
  }
  auto nout = node->AddOutputNode();
  {
    struct B {
      std::string operator()() { return "This is B."; };
    };

    B b;
    if (auto r = nout.borrow()) r->Bind(b);
  }
  EXPECT_EQ("This is A.", nin.borrow()->value().value().Get<std::string>());
  EXPECT_EQ("This is B.", nout.borrow()->value().value().Get<std::string>());

  { node->AddInputNode(); }

  EXPECT_EQ(2, node->GetPinInputRef().size());
  EXPECT_EQ(1, node->GetPinOutputRef().size());
}

TEST(NletTest, nlet1) {
  std::string hh("hello");
  nlet<std::string> iv(hh);
  auto w = iv.ref();
  if (auto r = w.borrow()) EXPECT_EQ(std::string("hello"), *r);
}

TEST(SequencerTest, case1) {
  auto* seq = new NSequencer();
  auto node1 = seq->AddNode();
  auto node2 = seq->AddNode();
  if (auto r = node1.borrow()) {
    r->BindToNextNode(node2);
    r->SetImplementation(
        [](PackedParentRef<NNodePinBase>, PackedParentRef<NNodePinBase>) {
          std::cout << "node1" << std::endl;
          return;
        });
  }

  if (auto r = node2.borrow()) {
    r->SetImplementation(
        [](PackedParentRef<NNodePinBase>, PackedParentRef<NNodePinBase>) {
          std::cout << "node2" << std::endl;
          return;
        });
  }

  std::thread th([&]() {
    seq->Execute();
    return;
  });
  std::cout << "FInished." << std::endl;
  EXPECT_EQ(1, 1);
}