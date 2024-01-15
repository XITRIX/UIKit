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
    auto vc = new_shared<TextViewController>();

    container()->addSubview(vc->view());
    addChild(vc);
    vc->didMoveToParent(shared_from_this());
}

void NavigationViewController::viewDidLayoutSubviews() {
    float extraNavigationTopMargin = 0;
    if (!view()->superview().expired()) {
        auto safeArea = view()->superview().lock()->safeAreaInsets();

        // Replicating iPhone 15 Pro Max behaviour, navigationBar's Y position is less than saveArea's top offset and equals 54
        extraNavigationTopMargin = fmaxf(safeArea.top - 54, 0);

        navigationBar()->yoga->setMarginTop(YGValue { safeArea.top - extraNavigationTopMargin, YGUnitPoint });
        tabBar()->yoga->setMarginBottom(YGValue { safeArea.bottom, YGUnitPoint });
    }
    setAdditionalSafeAreaInsets(UIEdgeInsets(navigationBar()->bounds().size.height - extraNavigationTopMargin, 0, tabBar()->bounds().size.height, 0));
}
