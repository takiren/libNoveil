#pragma once

#include <map>
#include <memory>

#include "SDL3/SDL.h"
namespace noveil {

template <class T>
class nlet;

template <class T>
class nref;

template <class T, class F>
class lock_val;

template <class T, class F>
class lock_val {
  friend class nref<T>;

 public:
  constexpr lock_val() noexcept = delete;
  lock_val(const lock_val&) noexcept = delete;
  lock_val& operator=(const lock_val&) noexcept = delete;

  lock_val(lock_val&&) noexcept = default;
  lock_val& operator=(lock_val&&) noexcept = default;
  ~lock_val() = default;

  T& operator*() const noexcept { return *m_sp; }
  T* operator->() const noexcept { return m_sp.get(); }

  explicit operator bool() const noexcept { return !!m_sp; }

 private:
  lock_val(const F& func) noexcept : m_sp(func()) {}

  std::shared_ptr<T> m_sp;
};

template <class T>
class nref {
 private:
  std::weak_ptr<T> m_wp;

 public:
  constexpr nref() = default;
  nref(const nref&) noexcept = default;
  nref& operator=(const nref&) noexcept = default;
  nref(nref&&) noexcept = default;
  nref& operator=(nref&&) noexcept = default;

  template <class Y>
  nref(const nref<Y>&& rhs) noexcept : m_wp(std::move(rhs.m_wp)){};

  template <class Y>
  nref& operator=(nref<Y>&& rhs) noexcept {
    m_wp = std::move(rhs.m_wp);
    return *this;
  }

  template <class Y>
  nref(const nlet<Y>& rhs) noexcept : m_wp(rhs.val) {}

  template <class Y>
  nref& operator=(const nlet<Y>& rhs) noexcept {
    ;
    return *this;
  }

  ~nref() = default;

  void swap(nref& rhs) noexcept { m_wp.swap(rhs.m_wp); }

  void reset() noexcept { m_wp.reset(); }

  long use_count() const noexcept { return m_wp.use_count(); }
  bool expired() const noexcept { return m_wp.expired(); }

  decltype(auto) borrow() noexcept {
    auto func = [this]() { return m_wp.lock(); };
    return lock_val<T, decltype(func)>(func);
  }

  const auto value() const {};
};

template <class T>
class nlet {
 private:
  std::shared_ptr<T> val;

 public:
  friend class nref<T>;
  nlet& operator=(const nlet&) = delete;
  template <class Y>
  nlet& operator=(const nlet<Y>&) = delete;

  template <class Y>
  nlet& operator=(const nlet<Y>&& rhs) noexcept {
    val = std::move(rhs.val);
    return *this;
  };

  template <class Y>
  nlet(const nlet<Y>&& rhs) noexcept : val(std::move(rhs.val)) {}

  constexpr nlet() : val(std::make_shared<T>()) {}
  template <class... Args>
  nlet(Args... args) noexcept : val(std::make_shared<T>(args...)) {}

  static nlet<T> create() { return nlet<T>(); };

  nref<T> ref() { return nref<T>(*this); }
};

}  // namespace noveil