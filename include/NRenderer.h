#pragma once

#include <SDL3/SDL.h>

#include "NUtils.h"

namespace noveil {
namespace render {
SDL_Window* windowRef;
SDL_Renderer* renderer;

void SetWindow(SDL_Window* wind);
void SetRenderer(SDL_Renderer* render);

}  // namespace render
}  // namespace noveil