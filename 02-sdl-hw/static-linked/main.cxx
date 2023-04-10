#include <iostream>

#include <SDL3/SDL_log.h>
#include <SDL3/SDL_version.h>

int main() {
  SDL_version compiled = {0, 0, 0};
  SDL_version linked = {};

  const int link_ver = SDL_GetVersion(&linked);

  if (link_ver != 0) {
    std::cerr << "Error: couldn't get SDL-linked version" << std::endl;
    return EXIT_FAILURE;
  }

  SDL_VERSION(&compiled);

  if (!compiled.major && !compiled.minor && !compiled.patch) {
    std::cerr << "Error: couldn't get SDL-compiled version" << std::endl;
    return EXIT_FAILURE;
  }

  SDL_Log("We compiled against SDL version %u.%u.%u\n", compiled.major, compiled.minor, compiled.patch);
  SDL_Log("We are STATICALLY linking against SDL version %u.%u.%u.\n", linked.major, linked.minor, linked.patch);

  return EXIT_SUCCESS;
}