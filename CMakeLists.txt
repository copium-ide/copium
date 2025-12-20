cmake_minimum_required(VERSION 3.20)
project(my_game LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_SCAN_FOR_MODULES ON)

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
set(BGFX_BUILD_TOOLS OFF CACHE BOOL "" FORCE)

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

FetchContent_MakeAvailable(SDL3 bgfx)

add_executable(my_game ${REGULAR_SRCS})

# activate modules
if(MOD_INTERFACES)
    target_sources(my_game PRIVATE 
        FILE_SET CXX_MODULES TYPE CXX_MODULES 
        FILES ${MOD_INTERFACES}
    )
endif()

# send unused code to narnia
if(MSVC)
    target_link_options(my_game PRIVATE /OPT:REF /OPT:ICF)
else()
    target_compile_options(my_game PRIVATE -ffunction-sections -fdata-sections)
    target_link_options(my_game PRIVATE -Wl,--gc-sections)
endif()

target_link_libraries(my_game PRIVATE 
    SDL3::SDL3 
    bgfx
)
