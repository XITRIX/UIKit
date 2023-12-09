cmake_minimum_required(VERSION 3.24)
project(UIKitDemo)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

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