//
//  IBTestViewController.hpp
//  SDLTest
//
//  Created by Даниил Виноградов on 14.03.2023.
//

#pragma once

#include <UIKit.hpp>

using namespace UIKit;

class IBTestViewController: public UIViewController {
public:
    void loadView() override;
    void viewDidLoad() override;
};
