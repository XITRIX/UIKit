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
    blank->setAlpha(0);
    blank->tag = "Blank";

    footerView = new_shared<UIView>();
    footerView->setBackgroundColor(UIColor::blue.withAlphaComponent(0.7f));
    footerView->tag = "View3";

    auto label = new_shared<UILabel>();
    label->setBackgroundColor(UIColor::red);
    label->yoga->setEnabled(true);
    label->font()->pointSize = 24;
    label->setText("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Maecenas eget nisl sed nibh tempus tincidunt. Proin gravida pretium justo eget sagittis. In at mi ut risus interdum tincidunt. Donec maximus convallis dolor sit amet malesuada. Proin lobortis neque a sem condimentum viverra. Duis in nulla at risus pretium volutpat. Praesent finibus, arcu non fringilla convallis, augue nisl tincidunt nibh, vel rutrum massa tortor ac neque. Vestibulum pellentesque velit nec erat consectetur lacinia. Nulla fringilla turpis a ligula faucibus, rhoncus dictum mi congue. Vestibulum felis ipsum, placerat nec lacus id, ornare pharetra ligula. Proin pulvinar bibendum mauris efficitur pretium. Proin cursus viverra eros quis scelerisque. Cras at pulvinar tortor.");
    contentView->addSubview(label);

    scrollView = new_shared<UIScrollView>();
    scrollView->setBackgroundColor(UIColor::cyan);
    contentView->addSubview(scrollView);

    root->addSubview(contentView);
    root->addSubview(headerView);
    root->addSubview(blank);
    root->addSubview(footerView);

    auto imageView = new_shared<UIImageView>(UIImage::fromPath("amogus.png"));
    scrollView->addSubview(imageView);
    imageView->setFrame(Rect(Point(), Size(300, 300)));
    imageView->setContentMode(UIViewContentMode::scaleAspectFit);

    setView(root);
}

void YogaTestViewController::viewDidLoad() {
    contentView->configureLayout([](std::shared_ptr<YGLayout> layout) {
        layout->setIncludedInLayout(false);
        layout->setFlexDirection(YGFlexDirectionColumn);
        layout->setAlignItems(YGAlignCenter);
        layout->setJustifyContent(YGJustifyCenter);
        layout->setRowGap(8);
    });

    scrollView->configureLayout([](std::shared_ptr<YGLayout> layout) {
        layout->setWidth(80_pt);
        layout->setHeight(80_pt);
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
