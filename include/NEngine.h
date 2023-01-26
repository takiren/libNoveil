#include <SDL.h>

namespace noveil {

/*!こいつからたたく*/
class NEngine {
 private:
  SDL_Window* windowRef;
  SDL_Renderer* rendererRef;

  int position, dir;

 public:
  NEngine() = delete;
  explicit NEngine(SDL_Window* window, SDL_Renderer* renderer);
  explicit NEngine(void* winId);
  virtual ~NEngine();

  void SetWindow(void* winId);

  SDL_Window* GetWindowRef() { return windowRef; }
  SDL_Renderer* GetRendererRef() { return rendererRef; }

  void Render();
};

}  // namespace noveil