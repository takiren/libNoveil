#pragma once

#include <cassert>
#include <map>
#include <memory>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

#include "NUtils.h"

namespace noveil {
class Variant;

using NVariantRef = std::shared_ptr<Variant>;
using PackedVariant = std::vector<Variant>;

template <typename KEY, typename DATA>
using hash_map = std::unordered_map<KEY, DATA>;

/*
 * std::variantのラッパークラス
 * @note void*にキャストするだけでもいい気がする。
 */

class Variant {
 private:
  /*!いずれプラグインなどで型が足りなくなったら使う用。もうちょいまともなアプローチを考える。*/
  using CustomVariant = void*;

  using RawVariant =
      std::variant<int, float, double, std::string, CustomVariant>;

  RawVariant variable;

 public:
  Variant() = default;
  virtual ~Variant() = default;

  Variant& operator=(const Variant& rhs) { this->variable = rhs.variable; }

  template <typename T>
  Variant(const T& rhs) {
    this->variable = rhs;
  }

  template <typename T>
  inline T Get() {
    assert(std::holds_alternative<T>(variable));
    return std::get<T>(variable);
  };

  template <typename T>
  bool bHasValue(T&& v) {
    return std::holds_alternative<T>(v);
  };

  inline bool operator==(const Variant& rhs) const {
    return this->variable == rhs.variable;
  };

  template <typename T>
  inline Variant& operator=(const T& rhs) {
    this->variable = rhs;
    return *this;
  }

  // CAUTION:It returns a copy of variable which type is std::variant.
  RawVariant GetRowVariant() { return variable; }
};

}  // namespace noveil
