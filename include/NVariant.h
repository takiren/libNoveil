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

class Variant {
 private:
  using CustomVariant = void*;
  using RawVariant =
      std::variant<int, float, double, std::string, CustomVariant>;

  RawVariant variable;

 public:
  explicit Variant() = default;
  virtual ~Variant() = default;
  Variant(const Variant&) = delete;
  

  Variant& operator=(const Variant& rhs) { this->variable = rhs.variable;
  }

  template<typename T>
  Variant(const T& rhs) {
    this->variable = rhs;
  }

  template <typename T>
  inline T Get() {
    assert(std::holds_alternative<T>(variable));
    return std::get<T>(variable);
  };

  template <typename T>
  bool bHasValue(T& v) {
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

template <>
inline int Variant::Get<int>() {
  // Assert when variable not held.
  assert(std::holds_alternative<int>(variable));
  return std::get<int>(variable);
}

}  // namespace noveil
