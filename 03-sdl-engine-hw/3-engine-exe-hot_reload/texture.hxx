#include <SDL3/SDL_video.h>
#include <iostream>
#include <vector>

#include <SDL3/SDL.h>
namespace my {
class texture {
  private:
    SDL_Texture* text;

  public:
    texture() = default;
    texture(SDL_Texture* t) {
      text = t;
    }
    SDL_Texture* get_texture() {
      return text;
    }
};

struct sdl_obj {
    SDL_Window* windows;
    SDL_Renderer* renderers;
    SDL_Surface* surfaces;
    SDL_Texture* textures;
    SDL_RWops* rwops;
};
} // namespace my