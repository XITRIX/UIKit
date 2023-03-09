//
//  TestViewController.hpp
//  SDLTest
//
//  Created by Даниил Виноградов on 06.03.2023.
//

#pragma once

#include <UIKit.hpp>

using namespace UIKit;

class TestViewController: public UIKit::UIViewController {
public:
    void loadView() override;
    void viewDidLoad() override;

    void touchesBegan(std::set<std::shared_ptr<UITouch>> touches, std::shared_ptr<UIEvent> event) override;
    
private:
    std::shared_ptr<UIKit::UIView> view1;
    std::shared_ptr<UIKit::UIView> view2;
    std::shared_ptr<UIKit::UIImageView> view3;
};
