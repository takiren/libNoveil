#pragma once

#include <filesystem>
#include <string>

#include "NUtils.h"
#include "NVariant.h"
namespace noveil {

namespace fs = std::filesystem;
class INAssetBase;  // インターフェース
class NAssetImage;

class FileInterface;

class FileInterface {
 private:
  fs::path filePath;

 public:
};

template <class T>
class AssetInterface;

template<class T>
class AssetInterface {
 private:
  T item;
};

class NTexture {

};

class INAssetBase : public INInfo {
 private:
  fs::path filePath;
  std::string name;

  Variant variable;

 public:
  explicit INAssetBase() = default;
  explicit INAssetBase(fs::path path) { filePath = path; };
  virtual ~INAssetBase() = default;

  virtual auto Load(fs::path) const -> decltype(this);
  virtual auto SaveAsAsset() const -> decltype(this);

  virtual std::string GetName() const { return name; }
};

class NAssetImage : public INAssetBase {
 private:
 protected:
 public:
  explicit NAssetImage() = default;
  explicit NAssetImage(fs::path path);

};

}  // namespace noveil