//
//  IBTestViewController.cpp
//  SDLTest
//
//  Created by Даниил Виноградов on 14.03.2023.
//

#include <IBTestViewController/IBTestViewController.hpp>

void IBTestViewController::loadView() {
    auto nib = UINib::fromPath("Layout/Test.xml");
    auto view = nib->instantiate();
    setView(view);
}

void IBTestViewController::viewDidLoad() {

}
