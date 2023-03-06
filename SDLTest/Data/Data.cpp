//
//  Data.cpp
//  SDLTest
//
//  Created by Даниил Виноградов on 25.02.2023.
//

#include <Data/Data.hpp>
#include <Utils/Utils.hpp>
#include <cstring>

namespace UIKit {

Data::Data(Uint8 bytes[], int count) {
    for (int i = 0; i < count; i++)
        _data.push_back(bytes[i]);
}

Data::~Data() {}

int Data::count() const {
    return (int) _data.size();
}

Uint8* Data::data() const {
    return (Uint8*) _data.data();
}

std::optional<Data> Data::fromPath(std::string path) {
    auto fileReader = SDL_RWFromFile((Utils::resourcePath + path).c_str(), "r");
    auto fileSize = int(fileReader->size(fileReader));

    Uint8* buffer = new Uint8[fileSize];

    auto bytesRead = int(fileReader->read(fileReader, buffer, 1, fileSize));

    fileReader->close(fileReader);

    if (bytesRead == fileSize) {
        return Data(buffer, fileSize);
    } else {
        return std::nullopt;
    }
}

}
