cmake_minimum_required(VERSION 3.20)
project(PixelEditor)

set(CMAKE_CXX_STANDARD 17)

#TO DO : setup SDL3 as a git submodule with Modern CMake

set(SDL3_DIR "C:/Libraries/SDL3")
set(SDL3_INCLUDE_DIR "${SDL3_DIR}/include")
set(SDL3_LIB_DIR "${SDL3_DIR}/lib/x64")


file(GLOB_RECURSE SRC_FILES src/*.cpp)

add_executable(pixel_editor ${SRC_FILES}
        src/main.cpp
        src/core/Image.cpp
        src/core/Image.h
        src/core/Layer.cpp
        src/core/Layer.h)

# TO DO: Dodaj SDL3 do kompilacji
target_include_directories(pixel_editor PRIVATE ${SDL3_INCLUDE_DIR})
target_link_directories(pixel_editor PRIVATE ${SDL3_LIB_DIR})
target_link_libraries(pixel_editor SDL3)