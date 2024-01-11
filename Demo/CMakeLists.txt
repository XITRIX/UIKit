include(${CMAKE_CURRENT_SOURCE_DIR}/lib/extern/cmake/toolchain.cmake)

cmake_minimum_required(VERSION 3.24)
project(UIKitDemo)

set(CMAKE_CXX_STANDARD 20)

#set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(VERSION_MAJOR "1")
set(VERSION_MINOR "0")
set(VERSION_ALTER "0")
set(VERSION_BUILD "0")
set(PACKAGE_NAME "ru.xitrix.SDLTest")
set(PROJECT_AUTHOR "XITRIX")
set(PROJECT_RESOURCES ${CMAKE_CURRENT_SOURCE_DIR}/assets)
set(PROJECT_ICON ${PROJECT_RESOURCES}/demo_icon.jpg)

add_definitions(-DSDL_GPU_DISABLE_OPENGL -DSDL_GPU_DISABLE_GLES_1 -DSDL_GPU_DISABLE_GLES_2)

if (USE_LIBROMFS)
    add_libromfs(${PROJECT_NAME} ${PROJECT_RESOURCES})
endif ()

add_executable(UIKitDemo
        main.cpp
        UIApplicationDelegate.cpp
        NavigationViewController/NavigationViewController.cpp
        Runner/Runner.cpp
        TestViewController/TestViewController.cpp
        TextViewController/TextViewController.cpp
        YogaTestViewController/YogaTestViewController.cpp
)

target_include_directories(UIKitDemo PRIVATE ${CMAKE_SOURCE_DIR})

add_subdirectory(lib)
target_link_libraries(UIKitDemo UIKit)

if (PLATFORM_IOS)
    ios_bundle(
            "ios/tvos/Splash.storyboard"
            "ios/iphoneos/Splash.storyboard"
            "ios/Images.xcassets"
            "ios/iOSBundleInfo.plist.in"
            "SDL_Test"
            "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_ALTER}")
elseif (PLATFORM_SWITCH)
    add_custom_target(${PROJECT_NAME}.nro DEPENDS ${PROJECT_NAME}
            COMMAND ${NX_NACPTOOL_EXE} --create "${PROJECT_NAME}" "${PROJECT_AUTHOR}" "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_ALTER}" ${PROJECT_NAME}.nacp --titleid=${PROJECT_TITLEID}
            COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_RESOURCES} ${CMAKE_BINARY_DIR}/resources
#            COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/resources/font
            COMMAND ${NX_ELF2NRO_EXE} ${PROJECT_NAME}.elf ${PROJECT_NAME}.nro --icon=${PROJECT_ICON} --nacp=${PROJECT_NAME}.nacp --romfsdir=${CMAKE_BINARY_DIR}/resources
    )
endif ()