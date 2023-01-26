#pragma once
#include "uuid_v4.h"
namespace noveil {
static UUIDv4::UUIDGenerator<std::mt19937_64> NUUIDGen;
using NUUID = UUIDv4::UUID;

//������p������uid����낤�I
/*!������p�����邾���ŃC���X�^���X��uid�����������*/
class INInfo {
 private:
  NUUID uid;
  std::string name;
  std::string description;
 public:
  explicit INInfo() : uid(NUUIDGen.getUUID()),name(""){};
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

}  // namespace noveil