message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")

function(check_libromfs_generator)
    if (NOT DEFINED LIBROMFS_PREBUILT_GENERATOR OR NOT EXISTS "${LIBROMFS_PREBUILT_GENERATOR}")
        if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/libromfs-generator")
            set(LIBROMFS_PREBUILT_GENERATOR "${CMAKE_CURRENT_SOURCE_DIR}/libromfs-generator" PARENT_SCOPE)
        else ()
            message(FATAL_ERROR "libromfs-generator has not been built, please refer to borealis/build_libromfs_generator.sh for more information")
        endif ()
    endif()
endfunction()

function(ios_bundle tvosStoryBoard iosStoryBoard assets plist name version)
    set(IOS_CODE_SIGN_IDENTITY "" CACHE STRING "The code sign identity to use when building the IPA.")
    set(IOS_GUI_IDENTIFIER "" CACHE STRING "The package name")
#    if(IOS_CODE_SIGN_IDENTITY STREQUAL "")
#        set(IOS_CODE_SIGNING_ENABLED NO)
#    else()
        set(IOS_CODE_SIGNING_ENABLED YES)
#    endif()
    if (IOS_GUI_IDENTIFIER STREQUAL "")
        set(IOS_GUI_IDENTIFIER "${PACKAGE_NAME}")
        message(WARNING "Using default package name: ${PACKAGE_NAME}")
    endif ()
    if (TVOS)
        set(IOS_SPLASH_STORYBOARD "${tvosStoryBoard}")
    else ()
        set(IOS_SPLASH_STORYBOARD "${iosStoryBoard}")
    endif ()
    set(IOS_ASSETS ${assets})
    set_target_properties(${PROJECT_NAME} PROPERTIES
            XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "${IOS_CODE_SIGN_IDENTITY}"
            XCODE_ATTRIBUTE_CODE_SIGNING_ALLOWED "${IOS_CODE_SIGNING_ENABLED}"
            XCODE_ATTRIBUTE_CODE_SIGNING_REQUIRED "${IOS_CODE_SIGNING_ENABLED}"
            BUNDLE TRUE
            MACOSX_BUNDLE_INFO_PLIST ${plist}
            MACOSX_BUNDLE TRUE
            MACOSX_BUNDLE_GUI_IDENTIFIER ${IOS_GUI_IDENTIFIER}
            MACOSX_BUNDLE_BUNDLE_NAME ${name}
            MACOSX_BUNDLE_BUNDLE_VERSION "${version}"
            MACOSX_BUNDLE_SHORT_VERSION_STRING "${version}"
            XCODE_ATTRIBUTE_ENABLE_BITCODE NO
            XCODE_ATTRIBUTE_SKIP_INSTALL NO
            XCODE_ATTRIBUTE_ASSETCATALOG_COMPILER_APPICON_NAME "AppIcon"
            XCODE_ATTRIBUTE_LD_RUNPATH_SEARCH_PATHS "@executable_path/Frameworks"
            XCODE_ATTRIBUTE_FRAMEWORK_SEARCH_PATHS "$(SRCROOT)/lib/extern/angle/ios"
            MACOSX_PACKAGE_LOCATION "$(SRCROOT)/assets"
            )
    set_property(
            SOURCE ${IOS_ASSETS}
            PROPERTY MACOSX_PACKAGE_LOCATION "Resources"
    )
    set_property(
            SOURCE ${IOS_SPLASH_STORYBOARD}
            PROPERTY MACOSX_PACKAGE_LOCATION "Resources"
    )
    target_sources(${PROJECT_NAME} PRIVATE
            ${IOS_ASSETS}
            ${IOS_SPLASH_STORYBOARD}
            )
endfunction()

function(add_libromfs app res)
    message(STATUS "USE LIBROMFS")
    add_definitions(-DUSE_LIBROMFS)
    set(LIBROMFS_PROJECT_NAME ${app} PARENT_SCOPE)
    set(LIBROMFS_RESOURCE_LOCATION "${res}" PARENT_SCOPE)
endfunction()

if (PLATFORM_MAC)
    message(STATUS "building for Mac")
    add_definitions(-DRESIZABLE_SCREEN)
    set(USE_LIBROMFS ON)
    check_libromfs_generator()
elseif (PLATFORM_IOS)
    message(STATUS "building for iOS")
    set(PLATFORM OS64)
    set(ENABLE_STRICT_TRY_COMPILE ON)
    set(DEPLOYMENT_TARGET 13.0)
    message(TOOLCHAIN PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib/extern/cmake/ios.toolchain.cmake)
    set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/lib/extern/cmake/ios.toolchain.cmake CACHE STRING "")
    set(USE_LIBROMFS ON)
    check_libromfs_generator()
elseif(PLATFORM_ANDROID)
    message(STATUS "building for Android")
    set(CMAKE_TOOLCHAIN_FILE $ENV{ANDROID_NDK_HOME}/build/cmake/android.toolchain.cmake CACHE PATH "android toolchain file")
    set(ANDROID_ABI arm64-v8a)
    set(USE_LIBROMFS ON)
    check_libromfs_generator()
elseif (PLATFORM_SWITCH)
    message(STATUS "building for SWITCH")
    if (NOT DEFINED ENV{DEVKITPRO})
        message(FATAL_ERROR "Please set DEVKITPRO in your environment. export DEVKITPRO=<path to>/devkitpro")
    endif ()
    set(DEVKITPRO $ENV{DEVKITPRO} CACHE BOOL "DEVKITPRO")
    set(__SWITCH__ ON)
    set(USE_SYSTEM_SDL ON)
    set(CMAKE_C_FLAGS "-I${DEVKITPRO}/libnx/include -I${DEVKITPRO}/portlibs/switch/include -D__SWITCH__")
    set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS}")
    include(${DEVKITPRO}/cmake/Switch.cmake REQUIRED)
    message("${DEVKITPRO}/portlibs/switch/include")
endif ()

function(add_libromfs app res)
    message(STATUS "USE LIBROMFS")
    add_definitions(-DUSE_LIBROMFS)
    set(LIBROMFS_PROJECT_NAME ${app} PARENT_SCOPE)
    set(LIBROMFS_RESOURCE_LOCATION "${res}" PARENT_SCOPE)
endfunction()