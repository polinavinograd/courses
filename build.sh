rm -r build
mkdir build
cd build
cmake -G "Ninja" ..
cmake --build .
cd 03-sdl-engine-hw/loop
./sdl-loop
