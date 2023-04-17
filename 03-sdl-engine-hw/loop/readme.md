SDL3 doesn't support using PNG-images. To build current project:

```bash
git clone https://github.com/libsdl-org/SDL_image.git
```

Configure CMake using CMake-GUI, generate CMake SDL_image_build.

```bash
cmake --build .
```

```bash
sudo cmake --install .
```