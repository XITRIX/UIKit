//
//  NavigationViewController.cpp
//  SDLTest
//
//  Created by Даниил Виноградов on 14.03.2023.
//

#include <NavigationViewController/NavigationViewController.hpp>
#include <TextViewController/TextViewController.hpp>

NavigationViewController::NavigationViewController() {
}

void NavigationViewController::loadView() {
    auto nib = UINib::fromPath("Layout/Navigation.xml");
    setView(nib->instantiate(&idStorage));
}

void NavigationViewController::viewDidLoad() {
    view()->setBackgroundColor(UIColor::cyan);

    auto vc = new_shared<TextViewController>();

    container()->addSubview(vc->view());
    addChild(vc);
    vc->didMoveToParent(shared_from_this());

    setAdditionalSafeAreaInsets(UIEdgeInsets(88, 0, 73, 0));
}
