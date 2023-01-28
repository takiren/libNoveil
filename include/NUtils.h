#pragma once
#include <memory>
#include <mutex>

#include "uuid_v4.h"

namespace noveil {
static UUIDv4::UUIDGenerator<std::mt19937_64> NUUIDGen;
using NUUID = UUIDv4::UUID;

// これを継承してuidを作ろう！
/*!これを継承するだけでインスタンスにuidが生成される*/
class INInfo : public std::enable_shared_from_this<INInfo> {
 private:
  NUUID uid;
  std::string name;
  std::string description;

 public:
  explicit INInfo() : uid(NUUIDGen.getUUID()), name(""){};
  virtual ~INInfo() = default;
  NUUID GetUID() const { return uid; }
  void SetName(const std::string& rhs) { this->name = rhs; }
  std::string GetName() const { return name; }
};

class Noncopyable {
 protected:
  Noncopyable() = default;
  virtual ~Noncopyable() = default;
  Noncopyable(Noncopyable&) = delete;
  Noncopyable& operator=(const Noncopyable&) = delete;
};

/*
 * @brief シングルトン用クラス
 * @details
 * @warnings
 * これを継承してシングルトンにする。継承先のコンストラクタをprotectedかprivateに変更。
 * @note スレッドセーフ。
 * @depricated あとでやめるかも。変に気取るより冗長化したほうがいいかも。
 */

template <class T>
class Singleton {
 private:
  static T* instance_;
  static std::mutex mutex_;

 protected:
  Singleton() = default;

 public:
  Singleton(const Singleton&) = delete;
  void operator=(const Singleton&) = delete;

  static T* GetInstance();
  virtual ~Singleton() = default;
};

/*!インスタンスがあるときは!nullptr*/
template <class T>
T* Singleton<T>::instance_ = nullptr;
/*!初期化*/
template <class T>
std::mutex Singleton<T>::mutex_;

/*!インスタンスを返す。*/
template <class T>
inline T* Singleton<T>::GetInstance() {
  std::lock_guard<std::mutex> lock(mutex_);
  if (!instance_) instance_ = new T();
  return instance_;
}

}  // namespace noveil