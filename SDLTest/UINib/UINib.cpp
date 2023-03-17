//
//  UINib.cpp
//  SDLTest
//
//  Created by Даниил Виноградов on 14.03.2023.
//

#include <UINib/UINib.hpp>
#include <Tools/Tools.hpp>
#include <UIView/UIView.hpp>
#include <tinyxml2.h>

namespace UIKit {

std::unordered_map<std::string, XIBViewCreator> UINib::xibViewsRegister;

void UINib::registerXIB(std::string name, XIBViewCreator creator) {
    xibViewsRegister[name] = creator;
}

std::shared_ptr<UINib> UINib::fromPath(std::string path) {
    auto data = Data::fromPath(path);
    if (!data.has_value()) return nullptr;
    return fromData(data.value());
}

std::shared_ptr<UINib> UINib::fromString(std::string rawData) {
    auto res = new_shared<UINib>();
    res->_rawXml = rawData;
    return res;
}

std::shared_ptr<UINib> UINib::fromData(Data data) {
    auto rawData = std::string((char*)data.data(), data.count());
    return fromString(rawData);
}

std::shared_ptr<UIView> UINib::instantiate(std::map<std::string, std::shared_ptr<UIView>>* idStorage) {
    // Load XML
    std::shared_ptr<tinyxml2::XMLDocument> document = new_shared<tinyxml2::XMLDocument>();
    tinyxml2::XMLError error        = document->Parse(_rawXml.c_str());

    if (error != tinyxml2::XMLError::XML_SUCCESS)
        fatal("Invalid XML when inflating XIB: error " + std::to_string(error));

    tinyxml2::XMLElement* element = document->RootElement();

    if (!element)
        fatal("Invalid XML: no element found");

    return UIView::instantiateFromXib(element, idStorage);
}

bool UINib::isEqual(std::shared_ptr<UINib> other) const {
    if (!other) return false;
    return _rawXml == other->_rawXml;
}

}
