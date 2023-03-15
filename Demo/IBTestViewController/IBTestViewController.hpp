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
    IBTestViewController();

    void loadView() override;
    void viewDidLoad() override;
private:
    std::shared_ptr<UINib> lastNib;
    std::shared_ptr<DispatchQueue> ibDispatch;
    std::shared_ptr<UIView> inflatedView;

    void refreshXib();
};
