//
//  Data.hpp
//  SDLTest
//
//  Created by Даниил Виноградов on 25.02.2023.
//

#pragma once

#include <SDL_gpu.h>
#include <memory>
#include <vector>
#include <string>
#include <optional>

namespace UIKit {

struct Data {
public:
    int count() const;
    Uint8* data() const;

    Data(Uint8* bytes, int count);
    ~Data();

    static std::optional<Data> fromPath(std::string path);
private:
    std::vector<Uint8> _data;
};

}
