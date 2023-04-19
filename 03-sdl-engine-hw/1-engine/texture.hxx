#include <iostream>
#include <vector>

#include <SDL3/SDL.h>

class texture {
  private:
    SDL_Texture* text;

  public:
    texture(SDL_Texture* t) {
      text = t;
    }
    SDL_Texture* get_texture() {
      return text;
    }
};

struct sdl_obj {
    std::vector<SDL_Window*> windows;
    std::vector<SDL_Renderer*> renderers;
    std::vector<SDL_Surface*> surfaces;
    std::vector<SDL_Texture*> textures;
    std::vector<SDL_RWops*> rwops;
};