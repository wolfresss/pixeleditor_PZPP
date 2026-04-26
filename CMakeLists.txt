cmake_minimum_required(VERSION 3.20)
project(PixelEditor)

set(CMAKE_CXX_STANDARD 17)

# SDL3 jako submodule
add_subdirectory(vendor/SDL)

# tylko .cpp!
file(GLOB_RECURSE SRC_FILES src/*.cpp)

add_executable(pixel_editor
    ${SRC_FILES}
    microui/src/microui.c
)

# include paths
target_include_directories(pixel_editor PRIVATE
    src
    microui/src
)

# linkowanie SDL
target_link_libraries(pixel_editor PRIVATE SDL3::SDL3)

#add_executable(pixel_editor ${SRC_FILES}###gdyby koniecznie dodać ręcznie, ale lepiej globem

 #       src/core/structs.cpp
  #      src/core/structs.h
   #     src/Tools/ITools.cpp
    #    src/Tools/ITools.h
     #   src/Renderer/View.cpp
      #  src/Renderer/View.h
       # microui/src/microui.c
        #microui/src/microui.h
       
    
#)

target_link_libraries(pixel_editor PRIVATE SDL3::SDL3)