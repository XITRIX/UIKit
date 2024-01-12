cmake_minimum_required(VERSION 3.24)
project(UIKit)

add_library(${PROJECT_NAME} STATIC
        SDLTest/Animations/CABasicAnimation/CABasicAnimation.cpp
        SDLTest/Animations/CABasicAnimationPrototype/CABasicAnimationPrototype.cpp
        SDLTest/Animations/CAMediaTimingFunction/CAMediaTimingFunction.cpp
        SDLTest/Animations/CASpringAnimation/CASpringAnimation.cpp
        SDLTest/Animations/CASpringAnimationPrototype/CASpringAnimationPrototype.cpp
        SDLTest/Animations/UIViewAnimationGroup/UIViewAnimationGroup.cpp
        SDLTest/Animations/UIViewAnimationOptions/UIViewAnimationOptions.cpp
        SDLTest/CADisplayLink/CADisplayLink.cpp
        SDLTest/CALayer/CALayer.cpp
        SDLTest/CATransaction/CATransaction.cpp
        SDLTest/CGImage/CGImage.cpp
        SDLTest/ContentsGravityTransformation/ContentsGravityTransformation.cpp
        SDLTest/Data/Data.cpp
        SDLTest/DispatchQueue/DispatchQueue.cpp
        SDLTest/FontRenderer/FontRenderer.cpp
        SDLTest/Geometry/Geometry.cpp
        SDLTest/NXAffineTransform/NXAffineTransform.cpp
        SDLTest/NXTransform3D/NXTransform3D.cpp
        SDLTest/Renderer/Renderer.cpp
        SDLTest/Shader/Shader.cpp
        SDLTest/ShaderProgram/ShaderProgram.cpp
        SDLTest/Timer/Timer.cpp
        SDLTest/Tools/OptionSet/OptionSet.cpp
        SDLTest/Tools/Fatal.cpp
        SDLTest/Tools/IBTools.cpp
        SDLTest/Tools/Tools.cpp
        SDLTest/UIApplication/UIApplication.cpp
        SDLTest/UIApplicationDelegate/UIApplicationDelegateImpl.cpp
        SDLTest/UIApplicationMain/UIApplicationMain.cpp
        SDLTest/UIColor/UIColor.cpp
        SDLTest/UIControl/UIControl.cpp
        SDLTest/UIControl/UIControlGestureRecognizer.cpp
        SDLTest/UIEdgeInsets/UIEdgeInsets.cpp
        SDLTest/UIEvent/UIEvent.cpp
        SDLTest/UIFocus/UIFocus.cpp
        SDLTest/UIFocusAnimationCoordinator/UIFocusAnimationCoordinator.cpp
        SDLTest/UIFocusSystem/UIFocusSystem.cpp
        SDLTest/UIFont/UIFont.cpp
        SDLTest/UIGestureRecognizer/UIGestureRecognizer.cpp
        SDLTest/UIImage/UIImage.cpp
        SDLTest/UIImageView/UIImageView.cpp
        SDLTest/UIKey/UIKey.cpp
        SDLTest/UILabel/UILabel.cpp
        SDLTest/UINib/UINib.cpp
        SDLTest/UIPanGestureRecognizer/UIPanGestureRecognizer.cpp
        SDLTest/UIPress/UIPress.cpp
        SDLTest/UIPressesEvent/UIPressesEvent.cpp
        SDLTest/UIRenderer/UIRenderer.cpp
        SDLTest/UIResponder/UIResponder.cpp
        SDLTest/UIScrollView/DecelerationTimingParameters.cpp
        SDLTest/UIScrollView/RubberBand.cpp
        SDLTest/UIScrollView/SpringTimingParameters.cpp
        SDLTest/UIScrollView/TimerAnimation.cpp
        SDLTest/UIScrollView/UIScrollView.cpp
        SDLTest/UITapGestureRecognizer/UITapGestureRecognizer.cpp
        SDLTest/UITouch/UITouch.cpp
        SDLTest/UIView/UIView.cpp
        SDLTest/UIViewController/UIViewController.cpp
        SDLTest/UIWindow/UIWindow.cpp
        SDLTest/ViewBuilder/ViewBuilder.cpp
        SDLTest/YogaExtensions/YGLayoutImpl.cpp
        SDLTest/YogaExtensions/YogaExtensions.cpp
)

# Platform Specific
if (PLATFORM_IOS)
    target_sources(${PROJECT_NAME} PRIVATE SDLTest/Platform/iOS/Platform.mm)
elseif (PLATFORM_SWITCH)
    target_sources(${PROJECT_NAME} PRIVATE SDLTest/Platform/Default/DefaultPlatform.cpp)
    target_sources(${PROJECT_NAME} PRIVATE SDLTest/Platform/Switch/switch_wrapper.c)
else ()
    target_sources(${PROJECT_NAME} PRIVATE SDLTest/Platform/Default/DefaultPlatform.cpp)
endif ()

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
if(USE_SYSTEM_SDL)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(SDL2 REQUIRED sdl2)
    message(STATUS "SDL2 Path: \"${SDL2_INCLUDE_DIRS}\"")
    target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})
    target_include_directories(${PROJECT_NAME} PUBLIC ${SDL2_INCLUDE_DIRS})
else ()
    target_include_directories(${PROJECT_NAME}
            PUBLIC
            extern/SDL/include
            extern/SDL-Headers
    )
    add_subdirectory(extern/SDL EXCLUDE_FROM_ALL)
    target_link_libraries(${PROJECT_NAME} SDL2-static SDL2main)
endif()

# libromfs
if(USE_LIBROMFS)
    add_subdirectory(extern/libromfs EXCLUDE_FROM_ALL)
#    list(APPEND BRLS_PLATFORM_LIBS ${LIBROMFS_LIBRARY})
    target_link_libraries(${PROJECT_NAME} ${LIBROMFS_LIBRARY})
endif()

# Angle / GL
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
elseif (PLATFORM_SWITCH)
    target_link_libraries(${PROJECT_NAME} EGL glapi GLESv2 drm_nouveau nx)
endif ()