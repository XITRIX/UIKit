//
//  YogaTestViewController.hpp
//  SDLTest
//
//  Created by Даниил Виноградов on 11.03.2023.
//

#pragma once

#include <UIKit.hpp>

using namespace UIKit;

class YogaTestViewController: public UIViewController {
public:
    void loadView() override;
    void viewDidLoad() override;

    void viewDidLayoutSubviews() override;
private:
    std::shared_ptr<UIView> view1;
    std::shared_ptr<UIView> view2;
    std::shared_ptr<UIView> view3;
    std::shared_ptr<UIView> blank;
};
