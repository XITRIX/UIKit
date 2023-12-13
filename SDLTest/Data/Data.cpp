//
//  Data.cpp
//  SDLTest
//
//  Created by Даниил Виноградов on 25.02.2023.
//

#include <Data/Data.hpp>
#include <Tools/Tools.hpp>

#ifdef USE_LIBROMFS
#include <romfs/romfs.hpp>
#endif

namespace UIKit {

Data::Data(Uint8 bytes[], int count, bool freeSource) {
    for (int i = 0; i < count; i++)
        _data.push_back(bytes[i]);

    if (freeSource)
        delete[] bytes;
}

Data::~Data() = default;

int Data::count() const {
    return (int) _data.size();
}

Uint8* Data::data() const {
    return (Uint8*) _data.data();
}

std::optional<Data> Data::fromPath(const std::string& path) {
#ifdef USE_LIBROMFS
    auto file = romfs::get(path);
    auto fileReader = SDL_RWFromConstMem(file.data(), (int) file.size());
#else
    auto fileReader = SDL_RWFromFile((Utils::resourcePath + path).c_str(), "r");
#endif

    auto fileSize = int(fileReader->size(fileReader));

    auto buffer = new Uint8[fileSize];

    auto bytesRead = int(fileReader->read(fileReader, buffer, 1, fileSize));

    fileReader->close(fileReader);

    if (bytesRead == fileSize) {
        return Data(buffer, fileSize, true);
    } else {
        delete[] buffer;
        return std::nullopt;
    }
}

}
