#include <SDL3/SDL.h>

int main(void) {
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    SDL_Log("SDL initialization failed: %s", SDL_GetError());
    return 1;
  }

  SDL_Window *window = SDL_CreateWindow("csynth", 640, 400, 0);

  if (window == NULL) {
    SDL_Log("window creation failed: %s", SDL_GetError());
    SDL_Quit();
    return 1;
  }

  SDL_Delay(3000);

  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
