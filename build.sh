rm -r build
mkdir build
cd build
cmake --no-warn-unused-cli -DCMAKE_EXPORT_COMPILE_COMMANDS=TRUE -DCMAKE_BUILD_TYPE=Debug -DCMAKE_C_COMPILER='/usr/bin/clang' -DCMAKE_CXX_COMPILER='/usr/bin/clang++' -G "Ninja" ..
cmake --build .

