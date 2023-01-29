#pragma once

#include <filesystem>
#include <memory>
#include <mutex>
#include <optional>
#include <string>

#include "NAsset.h"
#include "NMemory.h"
#include "NUtils.h"
#include "NVariant.h"
namespace noveil {

namespace fs = std::filesystem;
template <class T>
class NAssetManager final :public Singleton<NAssetManager<T>>{
 private:
  static NAssetManager* instance_;
  std::mutex mutex_;
 protected:
 public:
  static Load(fs::path path);
};

namespace asset {
std::vector<INAssetBase> PackedAssets;

hash_map<NUUID, INAssetBase> assetsMap;

template <class T>
std::weak_ptr<T> GetAsset(NUUID& uid){return static_cast < };

template <class T>
std::optional<std::weak_ptr<INAssetBase>> LoadAsset();

template <class T>
std::optional<std::weak_ptr<INAssetBase>> LoadAsset() {
  return std::optional<std::weak_ptr<INAssetBase>>();
}

}  // namespace asset

}  // namespace noveil