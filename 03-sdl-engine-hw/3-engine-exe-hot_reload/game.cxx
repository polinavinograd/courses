#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>

#include "engine.hxx"

class my_game : public game {
  public:
    void init(engine* eng) override { 
      game_eng = std::shared_ptr<engine>(eng); 
      image = game_eng->create_texture_from_file("/home/polinavngrd/Projects/courses/03-sdl-engine-hw/3-engine-exe-hot_reload/chelik.png");
    };
    void on_event() override {};
    void update() override {
      Uint32 ticks = game_eng->get_ticks();
      sprite_num = float((ticks / 100) % 4);
    }
    void render() const override {
      game_eng->fill_background_color(image, {200, 200, 200, 255});
      game_eng->render(image, {sprite_num * 32, 0, 32, 64}, {10, 10, 32, 64});
    }
  private:
    std::shared_ptr<engine> game_eng;
    texture image;
    float sprite_num;
};

game* create_game() {
  return std::make_shared<my_game>().get();
}
