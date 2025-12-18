#!/bin/bash
input_arg="${1:-debug}"
result="${input_arg^}"

MODS=$(find ./source -type f \( -name "*.ixx" -o -name "*.cppm" \))
SRCS=$(find ./source -type f -name "*.cpp" ! -name "*.ixx" ! -name "*.cppm")

echo "set(MOD_INTERFACES" > sources.cmake
for f in $MODS; do echo "    \"$f\"" >> sources.cmake; done
echo ")" >> sources.cmake

echo "set(REGULAR_SRCS" >> sources.cmake
for f in $SRCS; do echo "    \"$f\"" >> sources.cmake; done
echo ")" >> sources.cmake


cmake -G Ninja -B build -DCMAKE_BUILD_TYPE=$result
cmake --build build --config $result