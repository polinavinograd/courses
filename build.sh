rm -r build
mkdir build
cd build
cmake -G "Ninja" ..
cmake --build .
cd 03-sdl-engine-hw/engine-1
./sdl-engine-1
