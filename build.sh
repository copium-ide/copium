#!/bin/bash
set -e  # Exit on error

APP_NAME="my_game"

# Parse build type
input_arg="${1:-debug}"
result="${input_arg^}"

echo "Building $APP_NAME in $result mode..."


echo "Cleaning up previous build..."
rm -rf packaged/
mkdir -p packaged/assets/shaders/pbr/


echo "Scanning for source files..."
MODS=$(find ./source -type f \( -name "*.ixx" -o -name "*.cppm" \))
SRCS=$(find ./source -type f -name "*.cpp" ! -name "*.ixx" ! -name "*.cppm")

echo "set(MOD_INTERFACES" > sources.cmake
for f in $MODS; do echo "    \"$f\"" >> sources.cmake; done
echo ")" >> sources.cmake

echo "set(REGULAR_SRCS" >> sources.cmake
for f in $SRCS; do echo "    \"$f\"" >> sources.cmake; done
echo ")" >> sources.cmake

echo "Generated sources.cmake"


echo "Configuring with CMake..."
cmake -G Ninja -B build -DCMAKE_BUILD_TYPE=$result -Dname=$APP_NAME

echo "Building application..."
cmake --build build --config $result 


echo ""
echo "Compiling shaders..."

SHADERC="./build/_deps/bgfx-build/cmake/bgfx/shaderc"
BGFX_INCLUDE="./build/_deps/bgfx-src/bgfx/src"
SHADER_SRC="./source/assets/shaders/pbr"
SHADER_OUT="./packaged/assets/shaders/pbr"

# Wait a moment for filesystem to sync
sleep 0.5

# Check if shaderc exists
if [ ! -f "$SHADERC" ]; then
    echo "ERROR: shaderc not found at $SHADERC"
    echo ""
    echo "Looking for shaderc in build directory..."
    find ./build -name "shaderc" -type f 2>/dev/null || true
    echo ""
    echo "Possible solutions:"
    echo "1. Make sure BGFX_BUILD_TOOLS is ON in CMakeLists.txt"
    echo "2. Try running: cmake --build build --target shaderc"
    echo "3. Check if bgfx built successfully"
    exit 1
fi

# Make shaderc executable
chmod +x "$SHADERC"

# Verify shader source files exist
if [ ! -f "$SHADER_SRC/vs_pbr.sc" ]; then
    echo "ERROR: Vertex shader not found at $SHADER_SRC/vs_pbr.sc"
    exit 1
fi

if [ ! -f "$SHADER_SRC/fs_pbr.sc" ]; then
    echo "ERROR: Fragment shader not found at $SHADER_SRC/fs_pbr.sc"
    exit 1
fi

if [ ! -f "$SHADER_SRC/varying.def.sc" ]; then
    echo "ERROR: varying.def.sc not found at $SHADER_SRC/varying.def.sc"
    exit 1
fi

# Compile vertex shader
echo "  Compiling vs_pbr.sc..."
$SHADERC -f "$SHADER_SRC/vs_pbr.sc" -o "$SHADER_OUT/vs_pbr.bin" \
    --platform linux --type vertex --profile spirv \
    -i "$BGFX_INCLUDE" \
    --varyingdef "$SHADER_SRC/varying.def.sc" || {
        echo "ERROR: Failed to compile vertex shader"
        exit 1
    }

# Compile fragment shader
echo "  Compiling fs_pbr.sc..."
$SHADERC -f "$SHADER_SRC/fs_pbr.sc" -o "$SHADER_OUT/fs_pbr.bin" \
    --platform linux --type fragment --profile spirv \
    -i "$BGFX_INCLUDE" \
    --varyingdef "$SHADER_SRC/varying.def.sc" || {
        echo "ERROR: Failed to compile fragment shader"
        exit 1
    }

echo "✓ Shaders compiled successfully!" 

echo "Packaging application..."
cp build/$APP_NAME packaged/$APP_NAME

# Copy assets if they exist
if [ -d "packaged/assets" ]; then
    echo "Assets packaged successfully"
fi

echo ""
echo "=========================================="
echo "Build complete"
echo "------------------------------------------"
echo "Executable: packaged/$APP_NAME"
echo "Shaders:    packaged/assets/shaders/pbr/"
echo "            - vs_pbr.bin"
echo "            - fs_pbr.bin"
echo ""
echo "Run with: ./packaged/$APP_NAME"
echo "=========================================="