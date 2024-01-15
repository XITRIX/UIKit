//
//  TextViewController.hpp
//  SDLTest
//
//  Created by Даниил Виноградов on 17.03.2023.
//

#pragma once

#include <UIKit.hpp>

using namespace UIKit;

class TextViewController: public UIViewController {
public:
    void loadView() override;
    void viewDidLoad() override;
    void viewDidAppear(bool animated) override;

    void pressesBegan(std::set<std::shared_ptr<UIPress>> pressees, std::shared_ptr<UIPressesEvent> event) override;

    IBOutlet(UILabel, text)
    IBOutlet(UILabel, text1)
    IBOutlet(UILabel, text2)
    IBOutlet(UILabel, text3)
    IBOutlet(UIView, button)
    IBOutlet(UIView, dismissButton)
};
