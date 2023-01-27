/// @file
#pragma once

#include <filesystem>
#include <mutex>
#include <string>

#include "NGraph.h"
#include "NMemory.h"
#include "json.hpp"

namespace noveil {

class NFileIO {
 public:
  NFileIO() = default;
  virtual ~NFileIO() = default;

  virtual void Write() = 0;
  virtual void Read() = 0;
};

/*
 * @brief ハッシュマップのキーに対応する関数を実行したり管理するやつ。
 * @details funcHashMapに関数オブジェクトを登録。適当なキーから関数を呼び出す。
 * @tparam _ret 戻り値の型
 * @tparam _key ハッシュマップのキーの型
 * @tparam Args... 引数の型
 */

template <typename _ret, typename _key, typename... Args>
class NFileParserTemplate
    : public Singleton<NFileParserTemplate<_ret, _key, Args...>> {
 private:

  explicit NFileParserTemplate() = default;

 public:
  // TODO: Change val name.
  //@var funcHashMap _keyに対応した関数オブジェクトを格納。
  static hash_map<_key, std::function<_ret(Args...)>> funcHashMap;

  virtual ~NFileParserTemplate() = default;

  virtual void Read(){};
  /*
   * @brief ファイルから読み込み
   */
  inline _ret ReadFrom(_key key){};

  template <class Container>
  inline std::vector<_ret> ReadAll(Container&& items) {
    for (auto it = items.begin(); it != items.end(); ++it) {
    }
  };

  /*
   * @brief 関数オブジェクトを格納。
   */
  static void AddFunctor(_key key, std::function<_ret(Args...)>&& functor) {
    funcHashMap[key] = functor;
  }

  /*
   * @brief 関数オブジェクトをキーで呼び出し
   * @ret _ret
   */
  static _ret CallFunctor(_key key) { return std::move(funcHashMap[key]()); };
};

/*!
 * @brief funcHashMapの初期化。
 */
template <typename _ret, typename _key, typename... Args>
hash_map<_key, std::function<_ret(Args...)>>
    NFileParserTemplate<_ret, _key, Args...>::funcHashMap;

}  // namespace noveil