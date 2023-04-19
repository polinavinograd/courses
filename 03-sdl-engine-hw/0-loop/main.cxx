#include <cstdlib>
#include <iostream>
#include <vector>

#include <SDL3/SDL.h>
#include <SDL3/SDL_image.h>

int main(int argc, char* argv[]) {
  const std::vector<std::pair<SDL_Keycode, std::string>> keys {
    {     SDLK_w,       "up"},
    {     SDLK_s,     "down"},
    {     SDLK_a,     "left"},
    {     SDLK_d,    "right"},
    {     SDLK_e, "interact"},
    { SDLK_SPACE,     "fire"},
    {SDLK_ESCAPE,     "start"},
    {SDLK_LSHIFT,      "run"}
  };

  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0) {
    SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
    return EXIT_FAILURE;
  }

  SDL_Window* window = SDL_CreateWindow("SDL_mainloop", 800, 600, SDL_WINDOW_OPENGL);
  if (!window) {
    SDL_Log("Unable to create window: %s", SDL_GetError());
    SDL_Quit();
    return EXIT_FAILURE;
  }

  SDL_Renderer* renderer =
    SDL_CreateRenderer(window, NULL, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (!renderer) {
    SDL_Log("Unable to create renderer: %s", SDL_GetError());
    SDL_DestroyRenderer(renderer);
    renderer = NULL;
    SDL_DestroyWindow(window);
    window = NULL;
    SDL_Quit();
    return EXIT_FAILURE;
  }
  SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
  SDL_RenderClear(renderer);

  IMG_Init(IMG_INIT_PNG);
  SDL_RWops* io = SDL_RWFromFile("03-sdl-engine-hw/0-loop/chelik.png", "rb");
  if (!io) {
    SDL_Log("Unable to read file: %s", SDL_GetError());
    SDL_DestroyRW(io);
    io = NULL;
    SDL_DestroyRenderer(renderer);
    renderer = NULL;
    SDL_DestroyWindow(window);
    window = NULL;
    SDL_Quit();
    return EXIT_FAILURE;
  }
  SDL_Surface* image = IMG_LoadPNG_RW(io);
  if (!image) {
    SDL_Log("Unable to upload image: %s", SDL_GetError());
    SDL_DestroySurface(image);
    image = NULL;
    SDL_DestroyRW(io);
    io = NULL;
    SDL_DestroyRenderer(renderer);
    renderer = NULL;
    SDL_DestroyWindow(window);
    window = NULL;
    SDL_Quit();
    return EXIT_FAILURE;
  }

  SDL_Texture* image_texture = SDL_CreateTextureFromSurface(renderer, image);
  if (!image_texture) {
    SDL_Log("Unable to create texture from surface: %s", SDL_GetError());
    SDL_DestroySurface(image);
    image = NULL;
    SDL_DestroyTexture(image_texture);
    image_texture = NULL;
    SDL_DestroyRW(io);
    io = NULL;
    SDL_DestroyRenderer(renderer);
    renderer = NULL;
    SDL_DestroyWindow(window);
    window = NULL;
    SDL_Quit();
    return EXIT_FAILURE;
  }
  bool quit = false;

  while (!quit) {
    

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_EVENT_QUIT:
          quit = true;
          break;
        case SDL_EVENT_KEY_DOWN:
          for (int i = 0; i < keys.size(); i++) {
            if (keys[i].first == event.key.keysym.sym) {
              std::cout << keys[i].second << " is pressed\n";
              break;
            }
          }
          break;
        case SDL_EVENT_KEY_UP:
          for (int i = 0; i < keys.size(); i++) {
            if (keys[i].first == event.key.keysym.sym) {
              std::cout << keys[i].second << " is released\n";
              break;
            }
          }
          break;
      }
    }

    Uint32 ticks = SDL_GetTicks();
    Uint32 sprite = (ticks / 100) % 4;
    SDL_FRect srcrect = {float(sprite) * 32, 0, 32, 64};
    SDL_FRect dstrect = {10, 10, 32, 64};
    SDL_RenderClear(renderer);
    SDL_RenderTexture(renderer, image_texture, &srcrect, &dstrect);
    SDL_RenderPresent(renderer);
  }

  SDL_DestroySurface(image);
  image = NULL;
  SDL_DestroyTexture(image_texture);
  image_texture = NULL;
  SDL_DestroyRW(io);
  io = NULL;
  SDL_DestroyRenderer(renderer);
  renderer = NULL;
  SDL_DestroyWindow(window);
  window = NULL;
  SDL_Quit();

  return EXIT_SUCCESS;
}