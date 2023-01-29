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

namespace rsc {

namespace fs = std::filesystem;

template <class T>
hash_map<fs::path, std::shared_ptr<T>> resources;

template <class T>
std::optional<std::weak_ptr<T>> LoadAsset(fs::path path) {
  if (assets<T>[path]) return std::nullopt;
  assets<T>[path] = std::make_unique<T>(T::Create(path));
}
}  // namespace asset

}  // namespace noveil