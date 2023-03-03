//
//  Utils.hpp
//  SDLTest
//
//  Created by Даниил Виноградов on 09.02.2023.
//

#include <memory>
#include <cstdio>
#include <string>
#include <cassert>

#pragma once

namespace UIKit {

#define ptr std::shared_ptr
#define wptr std::weak_ptr

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

struct Utils {
    static std::string resourcePath;
};

}
