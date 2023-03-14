//
//  UINib.hpp
//  SDLTest
//
//  Created by Даниил Виноградов on 14.03.2023.
//

#pragma once

#include <Data/Data.hpp>
#include <string>
#include <functional>
#include <vector>
#include <unordered_map>

namespace UIKit {

class UIView;
typedef std::function<std::shared_ptr<UIView>()> XIBViewCreator;

class UINib {
public:
    static std::shared_ptr<UINib> fromPath(std::string path);
    static std::shared_ptr<UINib> fromString(std::string rawData);
    static std::shared_ptr<UINib> fromData(Data data);

    static void registerXIB(std::string name, XIBViewCreator creator);

    std::shared_ptr<UIView> instantiate();
private:
    std::string _rawXml;

    static std::unordered_map<std::string, XIBViewCreator> xibViewsRegister;

    friend class UIView;
};

}
