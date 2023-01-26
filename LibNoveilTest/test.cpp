#include "pch.h"

using namespace noveil;

// ノードとピンがとりあえず動くかどうか。
TEST(Graph, Simple) {
  NNodeExecutionTest1 t1;
  auto r = t1.func();
  EXPECT_EQ("Success", r.Get<std::string>());
}