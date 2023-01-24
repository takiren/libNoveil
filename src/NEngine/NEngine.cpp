#include "NEngine.h"
#include "NRenderer.h"
#include <SDL.h>

namespace noveil {

NEngine::NEngine(SDL_Window* window, SDL_Renderer* renderer) {
  windowRef = window;
  rendererRef = renderer;
}

NEngine::~NEngine() {
  SDL_DestroyRenderer(rendererRef);
  SDL_DestroyWindow(windowRef);
}
}  // namespace noveil
