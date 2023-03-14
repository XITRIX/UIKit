//
//  Utils.hpp
//  SDLTest
//
//  Created by Даниил Виноградов on 09.02.2023.
//

#include <any>
#include <set>
#include <vector>
#include <memory>
#include <cstdio>
#include <string>
#include <cassert>
#include <optional>
#include <functional>

#include <libretro-common/features/features_cpu.h>
#include <libretro-common/libretro.h>

#include <Tools/SharedBase.hpp>
#include <Tools/Fatal.hpp>
#include <Tools/IBTools.hpp>

#pragma once

namespace UIKit {

//#define ptr std::shared_ptr
//#define wptr std::weak_ptr

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
std::optional<T> any_optional_cast(std::optional<std::any> obj) {
    if (!obj.has_value()) { return std::nullopt; }

    try {
        return std::any_cast<T>(obj.value()); // throws
    }
    catch(const std::bad_any_cast& e) {
        return std::nullopt;
    }
}

template< typename T >
std::optional<T> firstWhere(std::vector<T> array, std::function<bool(T)> condition) {
    for (auto& item: array) {
        if (condition(item)) return item;
    }
    return std::nullopt;
}

template< typename T >
bool containsWhere(std::vector<T> array, std::function<bool(T)> condition) {
    for (auto& item: array) {
        if (condition(item)) return true;
    }
    return false;
}

template< typename T >
bool containsWhere(std::set<T> array, std::function<bool(T)> condition) {
    for (auto& item: array) {
        if (condition(item)) return true;
    }
    return false;
}

bool startsWith(const std::string& data, const std::string& prefix);
bool endsWith(const std::string& data, const std::string& suffix);

struct Utils {
    static std::string resourcePath;
};

}
