#include "pch.h"

#include"NVariant.h"
#include "NGraph.h"
using namespace noveil;
TEST(TestCaseName, TestName) {
  NNodeExecutionTest1 t1;
  auto r = t1.func();
  EXPECT_EQ("Success", r.Get<std::string>());
}