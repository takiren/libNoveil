#pragma once
#include "uuid_v4.h"
namespace noveil {
static UUIDv4::UUIDGenerator<std::mt19937_64> NUUIDGen;
using NUUID = UUIDv4::UUID;

//������p������uid����낤�I
/*!������p�����邾���ŃC���X�^���X��uid�����������*/
class INUid {
 private:
  NUUID uid;
 public:
  explicit INUid() : uid(NUUIDGen.getUUID()){};
  virtual ~INUid() = default;
  NUUID GetUID() const { return uid; }
};

class Noncopyable {
 protected:
  Noncopyable() = default;
  virtual ~Noncopyable() = default;
  Noncopyable(Noncopyable&) = delete;
  Noncopyable& operator=(const Noncopyable&) = delete;
};
}  // namespace noveil