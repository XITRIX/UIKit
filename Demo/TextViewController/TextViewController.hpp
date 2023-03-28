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

    IBOutlet(UILabel, text)
    IBOutlet(UIView, button)
    IBOutlet(UIView, dismissButton)
};
