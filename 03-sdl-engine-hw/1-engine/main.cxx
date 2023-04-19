#include <cstdlib>
#include <iostream>

#include "engine.hxx"

int main(int argc, char* argv[]) {
  std::unique_ptr<engine> engine = engine_builder::create_engine();

  texture image = engine->create_texture_from_file("03-sdl-engine-hw/1-engine/chelik.png");

  bool quit = false;
  while (!quit) {
    event event;
    Uint32 ticks = engine->get_ticks();
    float sprite = float((ticks / 100) % 4);
    while (engine->check_input(event)) {
      std::cout << event << std::endl;
      if (event == event::turn_off) {
        quit = true;
        break;
      }
    }
    engine->fill_background_color(image, {200, 200, 200, 255});
    engine->render(image, {sprite * 32, 0, 32, 64}, {10, 10, 32, 64});
  }

  engine_builder::destroy_engine(engine);
  return EXIT_SUCCESS;
}