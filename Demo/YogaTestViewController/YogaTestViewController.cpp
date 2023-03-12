//
//  YogaTestViewController.cpp
//  SDLTest
//
//  Created by Даниил Виноградов on 11.03.2023.
//

#include <YogaTestViewController/YogaTestViewController.hpp>


void YogaTestViewController::loadView() {
    auto root = new_shared<UIView>();

    headerView = new_shared<UIView>();
    headerView->setBackgroundColor(UIColor::red.withAlphaComponent(0.7f));
    headerView->tag = "View1";

    contentView = new_shared<UIView>();
    contentView->setBackgroundColor(UIColor::green);
    contentView->tag = "View2";
    contentView->setFitSuperview(true);

    blank = new_shared<UIView>();
    blank->tag = "Blank";

    footerView = new_shared<UIView>();
    footerView->setBackgroundColor(UIColor::blue.withAlphaComponent(0.7f));
    footerView->tag = "View3";

    auto label = new_shared<UILabel>();
    label->yoga->setEnabled(true);
    label->setText("Hello Mazafaka");

    contentView->addSubview(label);

    root->addSubview(contentView);
    root->addSubview(headerView);
    root->addSubview(blank);
    root->addSubview(footerView);

    setView(root);
}

void YogaTestViewController::viewDidLoad() {
    contentView->configureLayout([](std::shared_ptr<YGLayout> layout) {
        layout->setIncludedInLayout(false);
        layout->setAlignItems(YGAlignCenter);
        layout->setJustifyContent(YGJustifyCenter);
    });

    view()->configureLayout([](std::shared_ptr<YGLayout> layout) {
        layout->setFlexDirection(YGFlexDirectionColumn);
    });

    headerView->configureLayout([](std::shared_ptr<YGLayout> layout) {
        layout->setHeight(44_pt);
    });

    blank->configureLayout([](std::shared_ptr<YGLayout> layout) {
        layout->setFlexGrow(1);
    });

    footerView->configureLayout([](std::shared_ptr<YGLayout> layout) {
        layout->setHeight(44_pt);
    });
}
