//
//  TestViewController.hpp
//  SDLTest
//
//  Created by Даниил Виноградов on 06.03.2023.
//

#pragma once

#include <UIImageView/UIImageView.hpp>
#include <UIViewController/UIViewController.hpp>

class TestViewController: public UIKit::UIViewController {
public:
    void loadView() override;
    void viewDidLoad() override;
private:
    std::shared_ptr<UIKit::UIView> view1;
    std::shared_ptr<UIKit::UIView> view2;
    std::shared_ptr<UIKit::UIImageView> view3;
};
