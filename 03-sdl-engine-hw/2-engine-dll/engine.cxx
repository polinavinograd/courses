#include <memory>
#include <ostream>
#include <string>
#include <vector>

#include "engine.hxx"
#include <SDL3/SDL.h>
#include <SDL3/SDL_image.h>

std::ostream& operator<<(std::ostream& out, event e) {
  out << event_names.at(e);
  return out;
}

const std::vector<std::tuple<SDL_Keycode, std::string, event, event>> keys {
  {     SDLK_w,       "up",       event::up_pressed,       event::up_released},
  {     SDLK_s,     "down",     event::down_pressed,     event::down_released},
  {     SDLK_a,     "left",     event::left_pressed,     event::left_released},
  {     SDLK_d,    "right",    event::right_pressed,    event::right_released},
  {     SDLK_e, "interact", event::interact_pressed, event::interact_released},
  { SDLK_SPACE,     "fire",     event::fire_pressed,     event::fire_released},
  {SDLK_ESCAPE,    "start",    event::start_pressed,    event::start_released},
  {SDLK_LSHIFT,      "run",      event::run_pressed,      event::run_released}
};

bool engine_builder::exist = false;

std::unique_ptr<engine> engine_builder::create_engine() {
  if (exist) {
    throw "engine already exists!\n";
    return nullptr;
  }
  std::unique_ptr<engine> result = std::make_unique<engine>();
  bool init_res = result->system_init();
  if (init_res) {
    exist = true;
    return result;
  } else
    return nullptr;
}

void engine_builder::destroy_engine(const std::unique_ptr<engine>& eng) {
  if (!exist) {
    throw "engine doesn't exist!\n";
    return;
  }
  eng->system_deinit();
  exist = false;
}

bool engine::system_init() {
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0) {
    SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
    return false;
  }

  SDL_Window* window = SDL_CreateWindow("SDL_mainloop", 800, 600, SDL_WINDOW_OPENGL);
  if (!window) {
    SDL_Log("Unable to create window: %s", SDL_GetError());
    SDL_DestroyWindow(window);
    window = NULL;
    SDL_Quit();
    return false;
  }
  engine::objs->windows.emplace_back(window);

  SDL_Renderer* renderer =
    SDL_CreateRenderer(window, NULL, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (!renderer) {
    SDL_Log("Unable to create renderer: %s", SDL_GetError());
    SDL_DestroyRenderer(renderer);
    renderer = NULL;
    SDL_DestroyWindow(window);
    window = NULL;
    SDL_Quit();
    return false;
  }
  engine::objs->renderers.emplace_back(renderer);
  return true;
}

void engine::system_deinit() {
  if (this->objs->windows.size() != 0) {
    for (auto& window : objs->windows) {
      SDL_DestroyWindow(window);
    }
    this->objs->windows.clear();
  }
  if (this->objs->renderers.size() != 0) {
    for (auto& renderer : objs->renderers) {
      SDL_DestroyRenderer(renderer);
    }
    this->objs->renderers.clear();
  }
  if (this->objs->surfaces.size() != 0) {
    for (auto& surface : objs->surfaces) {
      SDL_DestroySurface(surface);
    }
    this->objs->surfaces.clear();
  }
  if (this->objs->textures.size() != 0) {
    for (auto& texture : objs->textures) {
      SDL_DestroyTexture(texture);
    }
    this->objs->textures.clear();
  }
  if (this->objs->rwops.size() != 0) {
    for (auto& rwop : objs->rwops) {
      SDL_DestroyRW(rwop);
    }
    this->objs->rwops.clear();
  }
  SDL_Quit();
}
bool engine::check_input(event& e) {
  SDL_Event event;
  if (SDL_PollEvent(&event)) {
    switch (event.type) {
      case SDL_EVENT_QUIT:
        e = event::turn_off;
        return true;
        break;
      case SDL_EVENT_KEY_DOWN:
        for (int i = 0; i < keys.size(); i++) {
          if (std::get<0>(keys[i]) == event.key.keysym.sym) {
            e = std::get<2>(keys[i]);
            return true;
          }
        }
        break;
      case SDL_EVENT_KEY_UP:
        for (int i = 0; i < keys.size(); i++) {
          if (std::get<0>(keys[i]) == event.key.keysym.sym) {
            e = std::get<3>(keys[i]);
            return true;
          }
        }
        break;
    }
  }
  return false;
}

texture engine::create_texture_from_file(const char* filename) {
  IMG_Init(IMG_INIT_PNG);
  SDL_RWops* io = SDL_RWFromFile(filename, "rb");
  if (!io) {
    SDL_Log("Unable to read file: %s", SDL_GetError());
    return nullptr;
  }
  engine::objs->rwops.emplace_back(io);

  SDL_Surface* image = IMG_LoadPNG_RW(io);
  if (!image) {
    SDL_Log("Unable to upload image: %s", SDL_GetError());
    return nullptr;
  }
  engine::objs->surfaces.emplace_back(image);

  SDL_Texture* image_texture = SDL_CreateTextureFromSurface(objs->renderers[0], image);
  if (!image_texture) {
    SDL_Log("Unable to create texture from surface: %s", SDL_GetError());
    return nullptr;
  }
  engine::objs->textures.emplace_back(image_texture);
  texture new_texture(image_texture);
  return new_texture;
}

void engine::fill_background_color(texture text, std::vector<int> rgba) {
  SDL_Renderer* renderer = objs->renderers[0];
  SDL_SetRenderDrawColor(renderer, rgba[0], rgba[1], rgba[2], rgba[3]);
  SDL_RenderClear(renderer);
}

Uint32 engine::get_ticks() {
  return SDL_GetTicks();
}

void engine::render(texture text, std::vector<float> src, std::vector<float> dst) {
  SDL_FRect srcrect = {src[0], src[1], src[2], src[3]};
  SDL_FRect dstrect = {dst[0], dst[1], dst[2], dst[3]};
  SDL_Renderer* renderer = objs->renderers[0];
  SDL_RenderClear(renderer);
  SDL_RenderTexture(renderer, text.get_texture(), &srcrect, &dstrect);
  SDL_RenderPresent(renderer);
}
