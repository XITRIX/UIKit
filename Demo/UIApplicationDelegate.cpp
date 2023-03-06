//
//  UIApplicationDelegate.cpp
//  SDLTest
//
//  Created by Даниил Виноградов on 06.03.2023.
//

#include <UIApplicationDelegate/UIApplicationDelegate.hpp>
#include <Utils/Utils.hpp>

namespace UIKit {

bool UIApplicationDelegate::applicationDidFinishLaunchingWithOptions(UIApplication* application, std::map<std::string, std::any> launchOptions) {

    // TODO: Replace with Bunbles
#ifdef __SWITCH__
    Utils::resourcePath = "romfs:/";
#elif __APPLE__
#include <TargetConditionals.h>
#if TARGET_IPHONE_SIMULATOR || TARGET_OS_IPHONE
    Utils::resourcePath = std::string(SDL_GetBasePath()) + "/assets/";
#endif
#endif

    window = std::make_shared<UIWindow>();
    window->makeKeyAndVisible();

    auto view1 = std::make_shared<UIKit::UIView>();
//    layer1->anchorPoint = Point(0, 0);
    view1->setFrame(Rect(44, 44, 280, 280));
    view1->setBackgroundColor(UIColor::blue);
    view1->layer()->cornerRadius = 16;
//    layer1->setAlpha(0.5f);
//    layer1->transform = NXTransform3D::translationBy(180, 180, 0);
//    layer1->transform = NXTransform3D::rotationBy(45, 0, 0, 1);// * NXTransform3D::translationBy(180, 180, 0);

    auto view2 = std::make_shared<UIKit::UIView>();
//    layer2->anchorPoint = Point(0.5f, 0.5f);
    view2->setFrame(Rect(40, 40, 80, 80));
    view2->setBackgroundColor(UIColor::red);//.withAlphaComponent(0.3f);
//    layer2->setOpacity(0.5f);
//    layer2->transform = NXTransform3D::rotationBy(45, 0, 0, 1);// * NXTransform3D::translationBy(0, 40, 0);
//    layer2->transform = NXTransform3D::scaleBy(2.f, 1, 0); //* NXTransform3D::translationBy(0, 40, 0);
//    layer2->transform = NXTransform3D::translationBy(20, 0, 0);//.concat(NXTransform3D::scaleBy(1.5f, 1, 0));


    auto view3 = std::make_shared<UIKit::UIView>();
    view3->setFrame(Rect(0, 0, 80, 80));
    view3->setBackgroundColor(UIColor::black);//.withAlphaComponent(0.5f);
    view3->layer()->cornerRadius = 16;
//    layer3->setTransform(NXTransform3D::rotationBy(45, 0, 0, 1) * NXTransform3D::scale(0.5f));

    auto imageData = Data::fromPath("test3.png");
    auto image = std::make_shared<CGImage>(imageData);

    view3->layer()->setContents(image);

    window->addSubview(view1);
//    layer1->setMask(layer2);
    view1->addSubview(view2);
//    layer2->setMask(layer3);
    view2->addSubview(view3);

    view3->setTransform(NXAffineTransform::rotationBy(-45) * NXAffineTransform::scale(2));

    UIView::animate(4, 2, UIViewAnimationOptions::curveEaseOutElastic, [view1, view3]() {
        view3->setBackgroundColor(UIColor::cyan);
        view3->setTransform(NXAffineTransform::rotationBy(45) * NXAffineTransform::scale(2));
//        view3->setTransform(NXAffineTransform::identity);

//        auto frame = view1->frame();
//        frame.origin = Point(344, 280);
//        view1->setFrame(frame);

        view1->setBackgroundColor(UIColor::orange);

//        view1->setTransform(NXAffineTransform::rotationBy(-125) * NXAffineTransform::scale(0.5f));
    }, [view1, view3](bool res) {
//        UIView::animate(4, 0, UIViewAnimationOptions::curveEaseOutElastic, [view1, view3]() {
//            view3->setBackgroundColor(UIColor::green);
//            view3->setTransform(NXAffineTransform::identity);
//
//            view1->setBackgroundColor(UIColor::cyan);
//            view1->setTransform(NXAffineTransform::identity);
//
//            auto frame = view1->frame();
//            frame.origin = Point(550, 69);
//            view1->setFrame(frame);
//
//        });
    });

    return true;
}

}
