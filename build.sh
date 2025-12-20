#!/bin/bash
APP_NAME="my_game"

# cleanup previous builds
rm -rf packaged/

# module tree definitions
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

# build app
cmake -G Ninja -B build -DCMAKE_BUILD_TYPE=$result -Dname=$APP_NAME
cmake --build build --config $result 

# build shaders
mkdir -p packaged/assets/shaders/pbr/
cd source/assets/shaders/pbr/
glslc -fshader-stage=frag pbr.frag.glsl -o ../../../../packaged/assets/shaders/pbr/pbr.frag.spv
glslc -fshader-stage=vert pbr.vert.glsl -o ../../../../packaged/assets/shaders/pbr/pbr.vert.spv
cd ../../../../

cp build/$APP_NAME packaged/$APP_NAME