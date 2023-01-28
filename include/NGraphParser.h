#pragma once
#include <magic_enum.hpp>
#include<mutex>

#include "NFileIO.h"
#include "NGraph.h"
namespace noveil {
enum NNode {
  PlayAnimation,
  Entry,
  PlaySound,
  Exit,
  PlayMusic,
  ChangeMusic,
  Transition
};
enum NPin {
  INT,
  FLOAT,
  DOUBLE,
  CUSTOM,
  STRING,
  PATH,
  NAME,
  CHARACTER,
  OBJECT,
  NONE
};

/*
 * @brief jsonファイルからNNodeBaseへ変換する。
 *
 */
class NGraphParser : public NFileParserTemplate<NNodeBase*, json> {
 private:
  static NGraphParser* instance;
  static std::mutex mutex_;
 public:
  NGraphParser() = delete;
  virtual ~NGraphParser() = default;
};

}  // namespace noveil