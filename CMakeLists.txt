cmake_minimum_required(VERSION 3.20)
project(copium LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_SCAN_FOR_MODULES ON)

# get name
if(NOT DEFINED name)
    set(name "my_game")
endif()

# use detected modules
include(${CMAKE_CURRENT_SOURCE_DIR}/sources.cmake)
set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS sources.cmake)

include(CheckIPOSupported)
check_ipo_supported(RESULT ipo_supported)
if(ipo_supported AND CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
endif()

set(SDL_STATIC ON)
set(SDL_SHARED OFF)
# wayland on for bgfx
set(WL_EGL_PLATFORM ON)
set(SDL_TEST OFF CACHE BOOL "" FORCE)
set(SDL_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BGFX_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BGFX_BUILD_TOOLS ON CACHE BOOL "" FORCE)  # Build shaderc

include(FetchContent)
FetchContent_Declare(
    SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG main
)
FetchContent_Declare(
    bgfx
    GIT_REPOSITORY https://github.com/bkaradzic/bgfx.cmake.git
    GIT_TAG master
)
FetchContent_Declare(
    bx
    GIT_REPOSITORY https://github.com/bkaradzic/bx.git
    GIT_TAG master
)
FetchContent_Declare(
    stb
    GIT_REPOSITORY https://github.com/nothings/stb.git
    GIT_TAG master
)

FetchContent_MakeAvailable(SDL3 bgfx bx stb)


add_executable(${name} ${REGULAR_SRCS})
include_directories(${bx_SOURCE_DIR}/include ${stb_SOURCE_DIR})


# activate modules
if(MOD_INTERFACES)
    target_sources(${name} PRIVATE 
        FILE_SET CXX_MODULES TYPE CXX_MODULES 
        FILES ${MOD_INTERFACES}
    )
endif()

# send unused code to narnia
if(MSVC)
    target_link_options(${name} PRIVATE /OPT:REF /OPT:ICF)
else()
    target_compile_options(${name} PRIVATE -ffunction-sections -fdata-sections)
    target_link_options(${name} PRIVATE -Wl,--gc-sections)
endif()

target_link_libraries(${name} PRIVATE 
    SDL3::SDL3 
    bgfx
    bx
)

install(TARGETS ${name} RUNTIME DESTINATION bin)
install(DIRECTORY ${SHADER_OUTPUT_DIR}/ DESTINATION bin/shaders)