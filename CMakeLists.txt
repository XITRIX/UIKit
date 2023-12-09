cmake_minimum_required(VERSION 3.24)
project(UIKit)

add_definitions(-DSDL_GPU_DISABLE_OPENGL -DSDL_GPU_DISABLE_GLES_1 -DSDL_GPU_DISABLE_GLES_3)

add_library(${PROJECT_NAME} STATIC)

file(GLOB_RECURSE UIKIT_SRC
        SDLTest/*.cpp)

target_sources(${PROJECT_NAME}
        PRIVATE
        ${UIKIT_SRC}
)

target_include_directories(${PROJECT_NAME}
        PUBLIC
        SDLTest
        SDLTest/Animations
)

target_include_directories(${PROJECT_NAME}
        PUBLIC
        extern/SDL/include
        extern/SDL-Headers
        extern/sdl-gpu/include
        extern/nanovg/src
        extern/libretro/include
        extern/yoga
        extern/tinyxml2
)

target_include_directories(${PROJECT_NAME}
        PUBLIC
        extern/angle/include
)

#file(GLOB_RECURSE SDL_GPU_SRC
#        extern/sdl-gpu/src/*.c)


target_include_directories(${PROJECT_NAME}
        PRIVATE
        extern/sdl-gpu/src/externals/stb_image_write
        extern/sdl-gpu/src
)

target_sources(${PROJECT_NAME}
        PRIVATE
        extern/sdl-gpu/src/SDL_gpu.c
        extern/sdl-gpu/src/SDL_gpu_matrix.c
        extern/sdl-gpu/src/SDL_gpu_renderer.c
        extern/sdl-gpu/src/SDL_gpu_shapes.c
        extern/sdl-gpu/src/renderer_GLES_2.c
        extern/sdl-gpu/src/externals/stb_image_write/stb_image_write.c
)

file(GLOB_RECURSE YOGA_SRC
        extern/yoga/yoga/*.cpp)

target_sources(${PROJECT_NAME}
        PRIVATE
        ${YOGA_SRC}
)

target_sources(${PROJECT_NAME}
        PRIVATE
        extern/nanovg/src/nanovg.c
)

target_sources(${PROJECT_NAME}
        PRIVATE
        extern/tinyxml2/tinyxml2.cpp
)

file(GLOB_RECURSE LIB_RETRO_SRC
        extern/libretro/src/*.c)

target_sources(${PROJECT_NAME}
        PRIVATE
        ${LIB_RETRO_SRC}
)

add_subdirectory(extern/SDL EXCLUDE_FROM_ALL)
target_link_libraries(${PROJECT_NAME} SDL2-static SDL2main)

target_link_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/extern/angle/mac)
target_link_libraries(${PROJECT_NAME} EGL)
target_link_libraries(${PROJECT_NAME} GLESv2)