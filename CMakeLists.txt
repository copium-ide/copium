cmake_minimum_required(VERSION 3.20)
project(my_game LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_SCAN_FOR_MODULES ON)

include(FetchContent)
FetchContent_Declare(
    SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG main
)
FetchContent_MakeAvailable(SDL3)

include(FetchContent)
FetchContent_Declare(
    DILIGENT
    GIT_REPOSITORY https://github.com/DiligentGraphics/DiligentEngine.git
    GIT_TAG master
)
FetchContent_MakeAvailable(DILIGENT)

file(GLOB_RECURSE ALL_SRCS "source/*.cpp")

set(MOD_INTERFACES "")
set(REGULAR_SRCS "")

foreach(src ${ALL_SRCS})
    file(STRINGS "${src}" matches REGEX "^[ \t]*export[ \t]+module[ \t]+" LIMIT_COUNT 1)
    
    if(matches)
        list(APPEND MOD_INTERFACES "${src}")
    else()
        list(APPEND REGULAR_SRCS "${src}")
    endif()
endforeach()

add_executable(my_game ${REGULAR_SRCS})

target_sources(my_game
    PRIVATE
        FILE_SET CXX_MODULES FILES
            ${MOD_INTERFACES}
)

target_link_libraries(my_game PRIVATE SDL3::SDL3)