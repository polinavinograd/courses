#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>

#include "engine.hxx"

namespace my {
class my_game : public game {
  public:
    void init(engine* eng) override {
      game_eng = eng;
      image = game_eng->create_texture_from_file(
        "../../../03-sdl-engine-hw/3-engine-exe-hot_reload/chelik.png");
    };
    void on_event(event event, bool& quit) override {
      std::cout << event << std::endl;
      if (event == event::turn_off) {
        quit = true;
      }
    };
    void update() override {
      Uint32 ticks = game_eng->get_ticks();
      sprite_num = float((ticks / 100) % 4);
    }
    void render() const override {
      game_eng->fill_background_color(image, {200, 200, 200, 255});
      game_eng->render(image, {sprite_num * 32, 0, 32, 64}, {10, 10, 32, 64});
    }

  protected:
    engine* game_eng;
    texture image;
    float sprite_num;
};

class new_game : public my_game {
  public:
    void update() override {
      Uint32 ticks = game_eng->get_ticks();
      sprite_num = float((ticks / 100) % 4);
    }
    void render() const override {
      game_eng->fill_background_color(image, {0, 200, 200, 255});
      game_eng->render(image, {sprite_num * 32, 0, 32, 64}, {10, 10, 32, 64});
    }
};

game* create_game() {
  return new new_game();
}
} // namespace my