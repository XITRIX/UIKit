//
//  NavigationViewController.hpp
//  SDLTest
//
//  Created by Даниил Виноградов on 14.03.2023.
//

#pragma once

#include <UIKit.hpp>

using namespace UIKit;

class NavigationViewController: public UIViewController {
public:
    NavigationViewController();

    void loadView() override;
    void viewDidLoad() override;

    void viewDidLayoutSubviews() override;

private:
    IBOutlet(UIView, container)
    IBOutlet(UIView, header)
    IBOutlet(UIView, navigationBar)
    IBOutlet(UIView, footer)
    IBOutlet(UIView, tabBar)

};
