#pragma once

#include"NMemory.h"
#include"NVariant.h"
#include"NAsset.h"
#include<string>
namespace noveil {
class NAssetManagerTmplBase {
 protected:
};

class NAssetManagerTmpl : public NAssetManagerTmplBase {
 public:
  template <typename T>
  inline T &Get() {
    return static_cast<T>();
  }
};

}  // namespace noveil