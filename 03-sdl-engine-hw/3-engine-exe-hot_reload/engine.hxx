#include <map>
#include <memory>
#include <string>
#include <vector>

#include "texture.hxx"

enum class event {
  left_pressed,
  left_released,
  right_pressed,
  right_released,
  up_pressed,
  up_released,
  down_pressed,
  down_released,
  interact_pressed,
  interact_released,
  start_pressed,
  start_released,
  run_pressed,
  run_released,
  fire_pressed,
  fire_released,
  turn_off
};

const std::map<event, std::string> event_names {
  {     event::left_pressed,      "left_pressed"},
  {    event::left_released,     "left_released"},
  {    event::right_pressed,     "right_pressed"},
  {   event::right_released,    "right_released"},
  {       event::up_pressed,        "up_pressed"},
  {      event::up_released,       "up_released"},
  {     event::down_pressed,      "down_pressed"},
  {    event::down_released,     "down_released"},
  { event::interact_pressed,  "interact_pressed"},
  {event::interact_released, "interact_released"},
  {    event::start_pressed,     "start_pressed"},
  {   event::start_released,    "start_released"},
  {      event::run_pressed,       "run_pressed"},
  {     event::run_released,      "run_released"},
  {     event::fire_pressed,      "fire_pressed"},
  {    event::fire_released,     "fire_released"},
  {         event::turn_off,          "turn_off"}
};

std::ostream& operator<<(std::ostream& out, event e);

class engine {
  private:
    sdl_obj objs;

  public:
    bool system_init();
    bool check_input(event& e);
    texture create_texture_from_file(const char* filename);
    void fill_background_color(texture text, std::vector<int> rgba);
    void render(texture text, std::vector<float> src, std::vector<float> dst);
    Uint32 get_ticks();
    void system_deinit();
};

class engine_builder {
  private:
    static bool exist;

  public:
    static std::unique_ptr<engine> create_engine();
    static void destroy_engine(const std::unique_ptr<engine>& eng);
};

struct game {
  public:
    virtual ~game() = default;
    virtual void init(engine* eng) = 0;
    virtual void update() = 0;
    virtual void on_event() = 0;
    virtual void render() const = 0;
};
