#pragma once

#include"NMemory.h"
#include"NAsset.h"
#include<string>
namespace noveil {
class NAssetManagerTmplBase {
 protected:
  hash_map<std::string, NAssetBase> contents;
};

class NAssetManagerTmpl : public NAssetManagerTmplBase {
 public:
  template <typename T>
  inline T &Get() {
    return static_cast<T>();
  }
};

}  // namespace noveil