//
//  Data.hpp
//  SDLTest
//
//  Created by Даниил Виноградов on 25.02.2023.
//

#pragma once

#include <SDL_gpu.h>
#include <vector>
#include <string>
#include <optional>
#include <Utils/Utils.hpp>

namespace UIKit {

class Data {
public:
    int count() const;
    Uint8* data() const;

    Data(Uint8* bytes, int count);
    ~Data();

    static ptr<Data> fromPath(std::string path);
private:
    int _count;
    Uint8* _data = nullptr;
};

}
