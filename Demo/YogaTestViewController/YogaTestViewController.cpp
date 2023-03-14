//
//  YogaTestViewController.cpp
//  SDLTest
//
//  Created by Даниил Виноградов on 11.03.2023.
//

#include <YogaTestViewController/YogaTestViewController.hpp>


void YogaTestViewController::loadView() {
//    auto nib = UINib::fromPath("Layout/Test.xml");
//    auto view = nib->instantiate();

    auto root = new_shared<UIView>();

    headerView = new_shared<UIView>();
    headerView->setBackgroundColor(UIColor::red.withAlphaComponent(0.7f));
    headerView->tag = "HeaderView";

    auto headerStack = new_shared<UIView>();
    headerStack->configureLayout([](std::shared_ptr<YGLayout> layout) {
        layout->setFlexDirection(YGFlexDirectionRow);
        layout->setPaddingLeft(65_pt);
    });

    contentView = new_shared<UIView>();
    contentView->setBackgroundColor(UIColor::green);
    contentView->tag = "ContentView";

    footerView = new_shared<UIView>();
    footerView->setBackgroundColor(UIColor::blue.withAlphaComponent(0.7f));
    footerView->tag = "FooterView";

    auto label = new_shared<UILabel>();
    label->setBackgroundColor(UIColor::red);
    label->configureLayout([](auto) {});
    label->font()->pointSize = 24;
    label->setText("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Maecenas eget nisl sed nibh tempus tincidunt. Proin gravida pretium justo eget sagittis. In at mi ut risus interdum tincidunt. Donec maximus convallis dolor sit amet malesuada. Proin lobortis neque a sem condimentum viverra. Duis in nulla at risus pretium volutpat. Praesent finibus, arcu non fringilla convallis, augue nisl tincidunt nibh, vel rutrum massa tortor ac neque. Vestibulum pellentesque velit nec erat consectetur lacinia. Nulla fringilla turpis a ligula faucibus, rhoncus dictum mi congue. Vestibulum felis ipsum, placerat nec lacus id, ornare pharetra ligula. Proin pulvinar bibendum mauris efficitur pretium. Proin cursus viverra eros quis scelerisque. Cras at pulvinar tortor.");

    auto imageView = new_shared<UIImageView>(UIImage::fromPath("amogus.png"));
//    imageView->setFrame(Rect(Point(), Size(300, 300)));
    imageView->setContentMode(UIViewContentMode::scaleAspectFit);
    imageView->configureLayout([](std::shared_ptr<YGLayout> layout) {
//        layout->setIncludedInLayout(false);
//        layout->setPositionType(YGPositionTypeAbsolute);
        layout->setWidth(300_pt);
        layout->setHeight(300_pt);
    });

    scrollView = new_shared<UIScrollView>();
    scrollView->setBackgroundColor(UIColor::cyan);
    scrollView->addSubview(imageView);

    contentView->addSubview(label);
    contentView->addSubview(scrollView);

    root->addSubview(contentView);
    root->addSubview(headerView);
    root->addSubview(footerView);

    setView(root);
}

void YogaTestViewController::viewDidLoad() {
    contentView->configureLayout([](std::shared_ptr<YGLayout> layout) {
        layout->setPositionType(YGPositionTypeAbsolute);
        layout->setWidth(100_percent);
        layout->setHeight(100_percent);
        layout->setFlexDirection(YGFlexDirectionColumn);
        layout->setAlignItems(YGAlignCenter);
        layout->setJustifyContent(YGJustifyCenter);
        layout->setRowGap(8);
    });

    scrollView->configureLayout([](std::shared_ptr<YGLayout> layout) {
        layout->setWidth(88_pt);
        layout->setHeight(80_pt);
    });

    view()->configureLayout([](std::shared_ptr<YGLayout> layout) {
        layout->setFlexDirection(YGFlexDirectionColumn);
        layout->setJustifyContent(YGJustifySpaceBetween);
    });

    headerView->configureLayout([](std::shared_ptr<YGLayout> layout) {
        layout->setHeight(8_pt);
    });

    footerView->configureLayout([](std::shared_ptr<YGLayout> layout) {
        layout->setHeight(73_pt);
    });
}
