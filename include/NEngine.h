#include <SDL.h>

namespace noveil {

class NEngine {
 private:
  SDL_Window* windowRef;
  SDL_Renderer* rendererRef;

 public:
  NEngine() = delete;
  NEngine(SDL_Window* window, SDL_Renderer* renderer);
  virtual ~NEngine();

  SDL_Window* GetWindowRef() { return windowRef; }
  SDL_Renderer* GetRendererRef() { return rendererRef; }
};

}  // namespace noveil