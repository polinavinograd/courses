rm -r build
mkdir build
cd build
cmake -G "Ninja" ..
cmake --build .
