#pragma once

#include <filesystem>
#include <string>
namespace noveil {
class NAssetBase;
class NAssetConverter;

class NAssetConverter {
 private:
 public:
  NAssetConverter() = default;
};

class NAssetBase {
 public:
  NAssetBase() = default;
  virtual ~NAssetBase() = default;
};

class NAssetImage {
 private:
  using Filepath = std::filesystem::path;

  //Path to file(like a.png, b.jpg, c.csv,...)
  Filepath fpath;

 public:
  NAssetImage(Filepath path);
  virtual ~NAssetImage() = default;
};
}  // namespace noveil