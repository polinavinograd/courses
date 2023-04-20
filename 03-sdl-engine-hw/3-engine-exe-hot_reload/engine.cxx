#include <SDL3/SDL_loadso.h>
#include <filesystem>
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

  objs.windows = SDL_CreateWindow("SDL_mainloop", 800, 600, SDL_WINDOW_OPENGL);
  if (!objs.windows) {
    SDL_Log("Unable to create window: %s", SDL_GetError());
    SDL_DestroyWindow(objs.windows);
    objs.windows = nullptr;
    SDL_Quit();
    return false;
  }

  objs.renderers =
    SDL_CreateRenderer(objs.windows, nullptr, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (!objs.renderers) {
    SDL_Log("Unable to create renderer: %s", SDL_GetError());
    SDL_DestroyRenderer(objs.renderers);
    objs.renderers = nullptr;
    SDL_DestroyWindow(objs.windows);
    objs.windows = nullptr;
    SDL_Quit();
    return false;
  }
  return true;
}

void engine::system_deinit() {
  if (objs.windows != nullptr) {
      SDL_DestroyWindow(objs.windows);
      objs.windows = nullptr;
    }    
  if (objs.renderers != nullptr) {
    SDL_DestroyRenderer(objs.renderers);
    objs.renderers = nullptr;
  }
  if (objs.surfaces!= nullptr) {
      SDL_DestroySurface(objs.surfaces);
          objs.surfaces = nullptr;

    }
  if (objs.textures !=nullptr) {
      SDL_DestroyTexture(objs.textures);
          objs.textures = nullptr;

    }
  if (objs.rwops != nullptr) {
      SDL_DestroyRW(objs.rwops);
    }
    objs.rwops = nullptr;
  
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
  objs.rwops = io;

  SDL_Surface* image = IMG_LoadPNG_RW(io);
  if (!image) {
    SDL_Log("Unable to upload image: %s", SDL_GetError());
    return nullptr;
  }
  objs.surfaces = image;

  SDL_Texture* image_texture = SDL_CreateTextureFromSurface(objs.renderers, image);
  if (!image_texture) {
    SDL_Log("Unable to create texture from surface: %s", SDL_GetError());
    return nullptr;
  }
  objs.textures = image_texture;
  texture new_texture(image_texture);
  return new_texture;
}

void engine::fill_background_color(texture text, std::vector<int> rgba) {
  SDL_Renderer* renderer = objs.renderers;
  SDL_SetRenderDrawColor(renderer, rgba[0], rgba[1], rgba[2], rgba[3]);
  SDL_RenderClear(renderer);
}

Uint32 engine::get_ticks() {
  return SDL_GetTicks();
}

void engine::render(texture text, std::vector<float> src, std::vector<float> dst) {
  SDL_FRect srcrect = {src[0], src[1], src[2], src[3]};
  SDL_FRect dstrect = {dst[0], dst[1], dst[2], dst[3]};
  SDL_Renderer* renderer = objs.renderers;
  SDL_RenderClear(renderer);
  SDL_RenderTexture(renderer, text.get_texture(), &srcrect, &dstrect);
  SDL_RenderPresent(renderer);
}

std::shared_ptr<game> reload_game(game* old,
                                  engine* eng,
                                  std::string lib,
                                  std::string temp_lib,
                                  void*& game_handle) {
  if (old) {
    SDL_UnloadObject(game_handle);
    game_handle = nullptr;
  }
  if (std::filesystem::exists(temp_lib.c_str())) {
    std::remove(temp_lib.c_str());
  }
  std::filesystem::copy(lib.c_str(), temp_lib.c_str());

  void* lib_handle = SDL_LoadObject(temp_lib.c_str());
  if (!lib_handle) {
    SDL_Log("Unable to load library handle: %s", SDL_GetError());
    return nullptr;
  }

  game_handle = lib_handle;
  auto create_game =
    reinterpret_cast<game* (*)()>(SDL_LoadFunction(lib_handle, "_Z11create_gamev"));
  if (!create_game) {
    SDL_Log("Unable to find game creator in %s: %s", temp_lib.c_str(), SDL_GetError());
    return nullptr;
  }

  auto new_game = create_game();
  if (!new_game) {
    SDL_Log("Unable create game from %s : %s", temp_lib.c_str(), SDL_GetError());
    return nullptr;
  }
  new_game->init(eng);
  return std::shared_ptr<game>(new_game);
}

// main function for hot reload

int main(int argc, char* argv[]) {
  std::unique_ptr<engine> engine = engine_builder::create_engine();

  std::string libname = "/home/polinavngrd/Projects/courses/build/03-sdl-engine-hw/3-engine-exe-hot_reload/libgame_lib.so";
  std::string templib = "/home/polinavngrd/Projects/courses/build/03-sdl-engine-hw/3-engine-exe-hot_reload/templib.so";

  void* game_lib_handle = nullptr;
  std::shared_ptr<game> current_game = reload_game(nullptr, engine.get(), libname, templib, game_lib_handle);

  auto time = std::filesystem::last_write_time(libname);

  bool quit = false;
  while (!quit) {
    event event;
    auto last_changed = std::filesystem::last_write_time(libname);
    if (last_changed != time) {
      current_game = reload_game(current_game.get(), engine.get(), libname, templib, game_lib_handle);
      if (!current_game) {
        std::cerr << "Unable to reload game!" << std::endl;
        continue;
      }
    }
    while (engine->check_input(event)) {
      current_game->on_event();
    }
    current_game->update();
    current_game->render();
  }

  engine_builder::destroy_engine(engine);
  return EXIT_SUCCESS;
}
