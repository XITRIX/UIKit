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
private:
    std::shared_ptr<UIView> headerView;
    std::shared_ptr<UIView> contentView;
    std::shared_ptr<UIView> footerView;
    std::shared_ptr<UIView> blank;
    std::shared_ptr<UIScrollView> scrollView;
};
