cmake_minimum_required(VERSION 3.20)
project(PixelEditor)

set(CMAKE_CXX_STANDARD 17)

# Dodaj SDL3 jako submodule
add_subdirectory(vendor/SDL)  # SDL3 musi mieć własny CMakeLists.txt

file(GLOB_RECURSE SRC_FILES src/*.cpp src/*.h)

add_executable(pixel_editor ${SRC_FILES})

target_link_libraries(pixel_editor PRIVATE SDL3::SDL3)