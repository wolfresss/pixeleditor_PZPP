set(CMAKE_POLICY_VERSION_MINIMUM "3.5")

cmake_minimum_required(VERSION 3.20)
project(PixelEditor)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POLICY_DEFAULT_CMP0135 NEW)

include(FetchContent)

FetchContent_Declare(
        freetype
        GIT_REPOSITORY https://github.com/freetype/freetype.git
        GIT_TAG        VER-2-13-2
        SOURCE_DIR     ${CMAKE_CURRENT_SOURCE_DIR}/vendor/SDL_ttf/external/freetype
)

FetchContent_Populate(freetype)

set(SDLTTF_HARFBUZZ OFF CACHE BOOL "" FORCE)
set(SDLTTF_PLUTOSVG OFF CACHE BOOL "" FORCE)
set(SDLTTF_VENDORED ON CACHE BOOL "" FORCE)

add_subdirectory(vendor/SDL)
add_subdirectory(vendor/SDL_ttf)

file(GLOB_RECURSE SRC_FILES src/*.cpp src/*.h src/*.c)

add_executable(pixel_editor ${SRC_FILES}
        microui/src/microui.c
        microui/src/microui.h
        src/Tools/Tools.cpp
        src/Tools/Tools.h
)

target_link_libraries(pixel_editor PRIVATE
        SDL3::SDL3
        SDL3_ttf::SDL3_ttf  # To automatycznie zlinkuje prawidłowy pod-target freetype
)

target_include_directories(pixel_editor PRIVATE
        microui/src
        vendor/SDL/include
        vendor/SDL_ttf/include
)