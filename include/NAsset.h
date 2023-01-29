#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_image.h>

#include <filesystem>
#include <optional>
#include <string>

#include "NRenderer.h"
#include "NUtils.h"
#include "NVariant.h"
#include "json.hpp"
namespace noveil {
namespace fs = std::filesystem;
using namespace nlohmann;

class INAssetBase;  // インターフェース
class NAssetImage;

class FileInterface;

class FileInterface {
 private:
  fs::path filePath;

 public:
};

class AssetInterface;

class AssetInterface : public INInfo {
 protected:
  fs::path filePath;
  MappedVariant<json> variants;

 public:
  AssetInterface() : filePath(""), INInfo(){};
};

class NTexture : public AssetInterface {
 private:
  SDL_Texture* texture;

 public:
  explicit NTexture() : AssetInterface(), texture(nullptr){};

  static NTexture* Create(fs::path path) {
    auto* instance = new NTexture();
    instance->filePath = path;
    instance->texture =
        IMG_LoadTexture(render::renderer, path.string().c_str());
  }
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

  template <class T>
  T Load(fs::path);

  virtual std::string GetName() const { return name; }
};

template <class T>
inline T noveil::INAssetBase::Load(fs::path) {
  return T();
}

class NAssetImage : public INAssetBase {
 private:
 protected:
 public:
  explicit NAssetImage() = default;
  explicit NAssetImage(fs::path path);
};

}  // namespace noveil