#pragma once

#include <filesystem>
#include<SDL3/SDL_image.h>

#include "SDL3/SDL.h"

namespace noveil {

namespace fs = std::filesystem;
struct NTexture;

struct NTexture {
  NTexture(fs::path path);

};
}  // namespace noveil