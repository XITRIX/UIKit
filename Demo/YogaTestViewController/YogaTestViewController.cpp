//
//  YogaTestViewController.cpp
//  SDLTest
//
//  Created by Даниил Виноградов on 11.03.2023.
//

#include <YogaTestViewController/YogaTestViewController.hpp>


void YogaTestViewController::loadView() {
    auto root = new_shared<UIView>();

    view1 = new_shared<UIView>();
    view1->setBackgroundColor(UIColor::red.withAlphaComponent(0.7f));
    view1->tag = "View1";

    view2 = new_shared<UIView>();
    view2->setBackgroundColor(UIColor::green);
    view2->tag = "View2";

    view2 = new_shared<UIView>();
    view2->setBackgroundColor(UIColor::green);
    view2->tag = "View2";

    blank = new_shared<UIView>();
    blank->tag = "Blank";

    view3 = new_shared<UIView>();
    view3->setBackgroundColor(UIColor::blue.withAlphaComponent(0.7f));
    view3->tag = "View3";

    root->addSubview(view2);
    root->addSubview(view1);
    root->addSubview(blank);
    root->addSubview(view3);

    setView(root);
}

void YogaTestViewController::viewDidLoad() {
    view()->configureLayout([](std::shared_ptr<YGLayout> layout) {
        layout->setEnabled(true);
        layout->setFlexDirection(YGFlexDirectionColumn);
    });

    view1->configureLayout([](std::shared_ptr<YGLayout> layout) {
        layout->setEnabled(true);
        layout->setHeight(44_pt);
    });

    blank->configureLayout([](std::shared_ptr<YGLayout> layout) {
        layout->setEnabled(true);
        layout->setFlexGrow(1);
    });

    view3->configureLayout([](std::shared_ptr<YGLayout> layout) {
        layout->setEnabled(true);
        layout->setHeight(44_pt);
    });

//    view()->yoga->applyLayoutPreservingOrigin(false);
}

void YogaTestViewController::viewDidLayoutSubviews() {
    view2->setFrame(view()->bounds());
}
