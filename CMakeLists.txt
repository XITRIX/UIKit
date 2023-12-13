cmake_minimum_required(VERSION 3.24)
project(UIKit)

add_library(${PROJECT_NAME} STATIC)

file(GLOB_RECURSE UIKIT_SRC
        SDLTest/*.cpp)

set (EXCLUDE_DIR "SDLTest/Platform")
foreach (TMP_PATH ${UIKIT_SRC})
    string (FIND ${TMP_PATH} ${EXCLUDE_DIR} EXCLUDE_DIR_FOUND)
    if (NOT ${EXCLUDE_DIR_FOUND} EQUAL -1)
        list (REMOVE_ITEM UIKIT_SRC ${TMP_PATH})
    endif ()
endforeach(TMP_PATH)

if (PLATFORM_IOS)
    list(APPEND UIKIT_SRC SDLTest/Platform/iOS/Platform.mm)
else ()
    list(APPEND UIKIT_SRC SDLTest/Platform/Default/DefaultPaltform.cpp)
endif ()

target_sources(${PROJECT_NAME}
        PRIVATE
        ${UIKIT_SRC}
)

target_include_directories(${PROJECT_NAME}
        PUBLIC
        SDLTest
        SDLTest/Animations
)

# SDL_Gpu
target_include_directories(${PROJECT_NAME}
        PUBLIC
        extern/sdl-gpu/include
        extern/sdl-gpu/src/externals/stb_image_write
        extern/sdl-gpu/src
)

target_sources(${PROJECT_NAME}
        PRIVATE
        extern/sdl-gpu/src/SDL_gpu.c
        extern/sdl-gpu/src/SDL_gpu_matrix.c
        extern/sdl-gpu/src/SDL_gpu_renderer.c
        extern/sdl-gpu/src/SDL_gpu_shapes.c
        extern/sdl-gpu/src/renderer_GLES_1.c
        extern/sdl-gpu/src/renderer_GLES_2.c
        extern/sdl-gpu/src/renderer_GLES_3.c
        extern/sdl-gpu/src/renderer_OpenGL_1.c
        extern/sdl-gpu/src/renderer_OpenGL_1_BASE.c
        extern/sdl-gpu/src/renderer_OpenGL_2.c
        extern/sdl-gpu/src/renderer_OpenGL_3.c
        extern/sdl-gpu/src/renderer_OpenGL_4.c
        extern/sdl-gpu/src/externals/stb_image_write/stb_image_write.c
)

# YOGA
target_include_directories(${PROJECT_NAME}
        PUBLIC
        extern/yoga
)

file(GLOB_RECURSE YOGA_SRC
        extern/yoga/yoga/*.cpp)

target_sources(${PROJECT_NAME}
        PRIVATE
        ${YOGA_SRC}
)

# NanoVG
target_include_directories(${PROJECT_NAME}
        PUBLIC
        extern/nanovg/src
)

target_sources(${PROJECT_NAME}
        PRIVATE
        extern/nanovg/src/nanovg.c
)

# TinyXML
target_include_directories(${PROJECT_NAME}
        PUBLIC
        extern/tinyxml2
)

target_sources(${PROJECT_NAME}
        PRIVATE
        extern/tinyxml2/tinyxml2.cpp
)

# LibRetro
target_include_directories(${PROJECT_NAME}
        PUBLIC
        extern/libretro/include
)

file(GLOB_RECURSE LIB_RETRO_SRC
        extern/libretro/src/*.c)

target_sources(${PROJECT_NAME}
        PRIVATE
        ${LIB_RETRO_SRC}
)

# SDL
target_include_directories(${PROJECT_NAME}
        PUBLIC
        extern/SDL/include
        extern/SDL-Headers
)

add_subdirectory(extern/SDL EXCLUDE_FROM_ALL)
target_link_libraries(${PROJECT_NAME} SDL2-static SDL2main)

# libromfs
if(USE_LIBROMFS)
    add_subdirectory(extern/libromfs EXCLUDE_FROM_ALL)
#    list(APPEND BRLS_PLATFORM_LIBS ${LIBROMFS_LIBRARY})
    target_link_libraries(${PROJECT_NAME} ${LIBROMFS_LIBRARY})
endif()

# Angle
if (PLATFORM_MAC)
    target_include_directories(${PROJECT_NAME}
            PUBLIC
            extern/angle/mac/include
    )

    target_link_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/extern/angle/mac)
    target_link_libraries(${PROJECT_NAME} EGL)
    target_link_libraries(${PROJECT_NAME} GLESv2)
elseif (PLATFORM_IOS)
    target_include_directories(SDL2-static
            PUBLIC
            extern/angle/ios/MetalANGLE.framework/Headers
    )
    target_include_directories(${PROJECT_NAME}
            PUBLIC
            extern/angle/ios/MetalANGLE.framework/Headers
    )

    set_target_properties(${PROJECT_NAME} PROPERTIES XCODE_EMBED_FRAMEWORKS_REMOVE_HEADERS_ON_COPY ON)
    set_target_properties(${PROJECT_NAME} PROPERTIES XCODE_EMBED_FRAMEWORKS_CODE_SIGN_ON_COPY ON)


endif ()