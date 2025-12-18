cmake_minimum_required(VERSION 3.20)
project(my_game LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_SCAN_FOR_MODULES ON)

include(${CMAKE_CURRENT_SOURCE_DIR}/sources.cmake)
set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS "sources.cmake")

# disable lto for dev builds
include(CheckIPOSupported)
check_ipo_supported(RESULT ipo_supported)
if(ipo_supported AND CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
endif()

# cut the fat from dependencies 
set(DILIGENT_BUILD_SAMPLES OFF CACHE BOOL "" FORCE)
set(DILIGENT_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(SDL_TEST OFF CACHE BOOL "" FORCE)
set(SDL_EXAMPLES OFF CACHE BOOL "" FORCE)

include(FetchContent)
FetchContent_Declare(SDL3 GIT_REPOSITORY https://github.com/libsdl-org/SDL.git GIT_TAG main)
FetchContent_Declare(DILIGENT GIT_REPOSITORY https://github.com/DiligentGraphics/DiligentEngine.git GIT_TAG master)
FetchContent_MakeAvailable(SDL3 DILIGENT)


add_executable(my_game ${REGULAR_SRCS})

# use list from build.sh instead of globs
if(MOD_INTERFACES)
    target_sources(my_game
        PRIVATE
            FILE_SET CXX_MODULES TYPE CXX_MODULES FILES ${MOD_INTERFACES}
    )
endif()

# send unused code to narnia
if(MSVC)
    target_link_options(my_game PRIVATE /OPT:REF)
else()
    target_compile_options(my_game PRIVATE -ffunction-sections -fdata-sections)
    target_link_options(my_game PRIVATE -Wl,--gc-sections)
endif()

target_link_libraries(my_game PRIVATE SDL3::SDL3 Diligent-GraphicsEngine)
