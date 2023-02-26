//
//  Data.cpp
//  SDLTest
//
//  Created by Даниил Виноградов on 25.02.2023.
//

#include <Data/Data.hpp>
#include <cstring>

namespace UIKit {

Data::Data(Uint8 bytes[], int count) {
    _data = new Uint8[count];
    std::memcpy(_data, bytes, count);
//    _data = bytes;
    _count = count;
}

Data::~Data() {
    if (_data) delete[] _data;
}

int Data::count() const {
    return _count;
}

Uint8* Data::data() const {
    return _data;
}

ptr<Data> Data::fromPath(std::string path) {
    auto fileReader = SDL_RWFromFile((Utils::resourcePath + path).c_str(), "r");
    auto fileSize = int(fileReader->size(fileReader));

    Uint8* buffer = new Uint8[fileSize];

    auto bytesRead = int(fileReader->read(fileReader, buffer, 1, fileSize));

    fileReader->close(fileReader);

    if (bytesRead == fileSize) {
        return std::make_shared<Data>(buffer, fileSize);
    } else {
        return nullptr;
    }
}

}
