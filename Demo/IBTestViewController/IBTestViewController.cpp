//
//  IBTestViewController.cpp
//  SDLTest
//
//  Created by Даниил Виноградов on 14.03.2023.
//

#include <IBTestViewController/IBTestViewController.hpp>

IBTestViewController::IBTestViewController() {
}

void IBTestViewController::loadView() {
    UIViewController::loadView();
    refreshXib();
}

void IBTestViewController::viewDidLoad() {
    view()->setBackgroundColor(UIColor::cyan);

    ibDispatch = new_shared<DispatchQueue>("IBDispatch");
    ibDispatch->async([this]() {
        while(true) {
            sleep(1000);
            DispatchQueue::main()->async([this]() {
                refreshXib();
            });
        }
    });
}

void IBTestViewController::refreshXib() {
    auto newNib = UINib::fromPath("Layout/Test.xml");
    if (newNib->isEqual(lastNib)) return;

    try {
        lastNib = newNib;
        if (inflatedView) inflatedView->removeFromSuperview();
        
        inflatedView = lastNib->instantiate();
        view()->addSubview(inflatedView);
    } catch(std::exception e) {
        auto errorNib = UINib::fromPath("Layout/Error.xml");
        inflatedView = errorNib->instantiate();
        view()->addSubview(inflatedView);
    }
}
