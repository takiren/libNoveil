/// @file
#pragma once

#include <filesystem>
#include <mutex>
#include <optional>
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
 * TODO:名前を変える。NFileって言ってるけどfileに限った話じゃないから。
 */
template <typename _ret, typename _key, typename... Args>
class NFileParserTemplate {
 private:
 protected:
 public:
  NFileParserTemplate() = delete;
  // TODO: Change val name.
  //@var funcHashMap _keyに対応した関数オブジェクトを格納。
  static hash_map<_key, std::function<_ret(Args...)>> funcHashMap;

  virtual ~NFileParserTemplate() = default;

  virtual void Read(){};
  /*
   * @brief ファイルから読み込み
   * @depricated
   */
  inline _ret ReadFrom(_key key, Args... args) {
    return funcHashMap[key](args...);
  };

  static inline _ret Parse(_key&& key, Args&&... args) {
    return funcHashMap[key](std::forward<Args>(args)...);
  };

  /*
   * @brief 関数オブジェクトを転送して格納。
   * @details 完全転送を行う。
   * @note msvcだとIDEでエラーが出るが、実際は出ない。
   *
   */
  static void AddFunctor(_key&& key, std::function<_ret(Args...)>&& functor) {
    funcHashMap[std::forward<_key>(key)] =
        std::forward<decltype(functor)>(functor);
  }

  /*
   * @brief 関数オブジェクトのコピーを格納。
   * @details コピーを行う。
   * @note visual studioだとエラーが出てるけどコンパイル通る。
   */
  static void AddFunctorCopy(_key key, std::function<_ret(Args...)> functor) {
    funcHashMap[key] = functor;
  }

  /*
   * @brief 関数オブジェクトをキーで呼び出し
   * @ret _ret
   * @note
   * std::moveしないと変数の寿命が尽きることがある(場合によるのでなんとも。)。
   * visual studioだとエラーが出てるけどコンパイル通る。
   */
  static inline _ret CallFunctor(_key&& key, Args&&... args) {
    return funcHashMap[key](std::forward<Args>(args)...);
  };
};

/*!
 * @brief funcHashMapの初期化。
 */
template <typename _ret, typename _key, typename... Args>
hash_map<_key, std::function<_ret(Args...)>>
    NFileParserTemplate<_ret, _key, Args...>::funcHashMap;

}  // namespace noveil