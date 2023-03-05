//
//  Utils.hpp
//  SDLTest
//
//  Created by Даниил Виноградов on 09.02.2023.
//

#include <any>
#include <memory>
#include <cstdio>
#include <string>
#include <cassert>
#include <optional>

#include <libretro-common/features/features_cpu.h>
#include <libretro-common/libretro.h>

#pragma once

namespace UIKit {

#define ptr std::shared_ptr
#define wptr std::weak_ptr

typedef retro_time_t Time;

/**
 * Returns the current CPU time in microseconds.
 */
inline Time getCPUTimeUsec()
{
    return cpu_features_get_time_usec();
}

template< typename... Args >
std::string string_sprintf( const char* format, Args... args ) {
  int length = std::snprintf( nullptr, 0, format, args... );
  assert( length >= 0 );

  char* buf = new char[length + 1];
  std::snprintf( buf, length + 1, format, args... );

  std::string str( buf );
  delete[] buf;
  return str;
}

void setAntialiasing(bool enabled);

template< typename T >
std::optional<T> any_optional_cast(std::any obj) {
    try {
        return std::any_cast<T>(obj); // throws
    }
    catch(const std::bad_any_cast& e) {
        return std::nullopt;
    }
}

struct Utils {
    static std::string resourcePath;
};

}
