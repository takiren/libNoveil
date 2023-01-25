#include "NEngine.h"

#include "NRenderer.h"

namespace noveil {

NEngine::NEngine(SDL_Window* window, SDL_Renderer* renderer) {
  SDL_Init(SDL_INIT_VIDEO);
  dir = 1;
  position = 0;
  windowRef = window;
  rendererRef = renderer;
}

NEngine::NEngine(void* winId) {
  SDL_Init(SDL_INIT_VIDEO);
  dir = 1;
  position = 0;
  windowRef = SDL_CreateWindowFrom(winId);
  rendererRef = SDL_CreateRenderer(windowRef, -1, SDL_RENDERER_ACCELERATED);
}

NEngine::~NEngine() {
  SDL_DestroyRenderer(rendererRef);
  SDL_DestroyWindow(windowRef);
  dir = 1;
  position = 0;
  windowRef = 0;
  rendererRef = 0;
}
void NEngine::SetWindow(void* winId) { SDL_CreateWindowFrom(winId); }
void NEngine::Render() {  // Basic square bouncing animation
  SDL_Rect spos;
  spos.h = 100;
  spos.w = 100;
  spos.y = 500 / 2 - 50;
  spos.x = position;

  SDL_SetRenderDrawColor(rendererRef, 0, 0, 0, 255);
  SDL_RenderFillRect(rendererRef, 0);
  SDL_SetRenderDrawColor(rendererRef, 0xFF, 0x0, 0x0, 0xFF);
  SDL_RenderFillRect(rendererRef, &spos);
  SDL_RenderPresent(rendererRef);

  if (position >= 500 - 100)
    dir = 0;
  else if (position <= 0)
    dir = 1;

  if (dir)
    position += 5;
  else
    position -= 5;
}

}  // namespace noveil
