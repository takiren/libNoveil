#pragma once

#include "NUtils.h"
#include "SDL.h"

namespace noveil {
class NMainRenderer : private Singleton<NMainRenderer> {
 private:
 protected:
 public:
  static void Init();
};
}  // namespace noveil