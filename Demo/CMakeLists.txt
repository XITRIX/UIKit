include(${CMAKE_CURRENT_SOURCE_DIR}/lib/extern/cmake/toolchain.cmake)

cmake_minimum_required(VERSION 3.24)
project(UIKitDemo)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(VERSION_MAJOR "1")
set(VERSION_MINOR "0")
set(VERSION_ALTER "0")
set(VERSION_BUILD "0")
set(PACKAGE_NAME "ru.xitrix.SDLTest")
set(PROJECT_AUTHOR "XITRIX")
set(PROJECT_RESOURCES ${CMAKE_CURRENT_SOURCE_DIR}/assets)

add_definitions(-DRESIZABLE_SCREEN)

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
endif ()