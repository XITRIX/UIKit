//
//  TestViewController.cpp
//  SDLTest
//
//  Created by Даниил Виноградов on 06.03.2023.
//

#include <TestViewController/TestViewController.hpp>

using namespace UIKit;

class TestView: public UIView {
    void safeAreaInsetsDidChange() override {
//        auto a = safeAreaInsets();
//        printf("Changed %f %f %f %f\n", a.top, a.left, a.bottom, a.right);
    }

    void layoutMarginsDidChange() override {
        auto a = layoutMargins();
        printf("Changed %f %f %f %f\n", a.top, a.left, a.bottom, a.right);
    }
};

void TestViewController::loadView() {
    auto view = new_shared<UIView>();
    view->yoga->setEnabled(false);

    rotationView = new_shared<UIView>(Rect(60, 200, 188, 188));
    rotationView->yoga->setEnabled(false);
    rotationView->setBackgroundColor(UIColor::orange);
//    rotationView->layer()->setCornerRadius(8);

    view1 = new_shared<TestView>();
    view1->setLayoutMargins(UIEdgeInsets(8, 8, 8, 8));
    view1->yoga->setEnabled(false);
    view1->tag = "View 1";
//    layer1->anchorPoint = Point(0, 0);
    view1->setFrame(Rect(44, 44, 280, 280));
    view1->setBackgroundColor(UIColor::blue);
    view1->layer()->setCornerRadius(16);
//    layer1->setAlpha(0.5f);
//    layer1->transform = NXTransform3D::translationBy(180, 180, 0);
//    layer1->transform = NXTransform3D::rotationBy(45, 0, 0, 1);// * NXTransform3D::translationBy(180, 180, 0);

    view2 = new_shared<UIKit::UIView>();
    view2->yoga->setEnabled(false);
    view2->tag = "View 2";
//    layer2->anchorPoint = Point(0.5f, 0.5f);
    view2->setFrame(Rect(40, 40, 80, 80));
    view2->setBackgroundColor(UIColor::red);//.withAlphaComponent(0.3f);
//    layer2->setOpacity(0.5f);
//    layer2->transform = NXTransform3D::rotationBy(45, 0, 0, 1);// * NXTransform3D::translationBy(0, 40, 0);
//    layer2->transform = NXTransform3D::scaleBy(2.f, 1, 0); //* NXTransform3D::translationBy(0, 40, 0);
//    layer2->transform = NXTransform3D::translationBy(20, 0, 0);//.concat(NXTransform3D::scaleBy(1.5f, 1, 0));


    view3 = new_shared<UIKit::UIImageView>(UIImage::fromPath("amogus.png"));
    view3->yoga->setEnabled(false);
    view3->tag = "View 3";
    view3->setFrame(Rect(0, 0, 80, 80));
    view3->setBackgroundColor(UIColor::black);//.withAlphaComponent(0.5f);
//    view3->layer()->cornerRadius = 16;
//    layer3->setTransform(NXTransform3D::rotationBy(45, 0, 0, 1) * NXTransform3D::scale(0.5f));

    view->addSubview(view1);
//    layer1->setMask(layer2);
    view1->addSubview(view2);
//    layer2->setMask(layer3);
    view2->addSubview(view3);

    label = new_shared<UILabel>();
    label->yoga->setEnabled(false);
    label->tag = "View Label";
    label->setFrame(Rect(480, 90, 300, 44));
    label->setBackgroundColor(UIColor::green);
    label->setNumberOfLines(0);
    label->setText("Helloooo!\nHelloooo!\nHelloooo!");
    label->font()->pointSize = 24;
    label->setClipsToBounds(true);
    label->sizeToFit();

    button = new_shared<UILabel>();
    button->yoga->setEnabled(false);
    button->tag = "Button";
    button->setFrame(Rect(300, 90, 300, 44));
    button->setBackgroundColor(UIColor::green);
    button->setText("Press me!");
    button->font()->pointSize = 24;
    button->sizeToFit();


    view->addSubview(label);
    view->addSubview(button);
    view->addSubview(rotationView);
    
    setView(view);
}

bool scaled = false;
void TestViewController::viewDidLoad() {
    setAdditionalSafeAreaInsets(UIEdgeInsets(88, 88, 88, 88));

    startRotate();

    auto tap = new_shared<UITapGestureRecognizer>();
    tap->onStateChanged = [this](auto state) {
        if (state == UIGestureRecognizerState::ended) {
            auto v = view();
            auto a = v->safeAreaInsets();

            UIView::animate(0.3f, [this]() {
                if (!scaled) {
                    label->setText("Press me!");
                    label->layer()->setCornerRadius(0);
                } else {
                    label->setText("Helloooo!\nHelloooo!\nHell!");
                    label->layer()->setCornerRadius(16);
                }
                label->sizeToFit();
            });
            scaled = !scaled;
        }
    };
    button->addGestureRecognizer(tap);

    auto pan = new_shared<UIPanGestureRecognizer>();
    pan->onStateChanged = [this, pan](auto state) {
//        printf("%d\n", state);
        switch (state) {
            case UIGestureRecognizerState::began: {
                initial = view1->frame().origin;
                break;
            }
            case UIGestureRecognizerState::changed: {
                auto translation = pan->translationInView(view());

                auto frame = view1->frame();
                frame.origin.x = initial.x + translation.x;
                frame.origin.y = initial.y + translation.y;
                view1->setFrame(frame);
                break;
            }
            default:
                break;
        }
    };
    view1->addGestureRecognizer(pan);

    DispatchQueue::main()->async([]() {
        printf("Test main dispatch\n");
    });


//    view3->setTransform(NXAffineTransform::rotationBy(-45) * NXAffineTransform::scale(2));

    auto options = UIViewAnimationOptions(curveEaseOutElastic | allowUserInteraction);
    UIView::animate(4, 2, options, [this]() {
        view3->setBackgroundColor(UIColor::cyan);
        view3->setTransform(NXAffineTransform::rotationBy(45) * NXAffineTransform::scale(2));
//        view3->setTransform(NXAffineTransform::identity);

        auto frame = view1->frame();
        frame.origin = Point(344, 280);
        view1->setFrame(frame);

        view1->setBackgroundColor(UIColor::orange);

//        view1->setTransform(NXAffineTransform::rotationBy(-125) * NXAffineTransform::scale(0.5f));
    }, [this, options](bool res) {
        UIView::animate(4, 0, options, [this]() {
            view3->setBackgroundColor(UIColor::green);
            view3->setTransform(NXAffineTransform::identity);

            view1->setBackgroundColor(UIColor::cyan);
            view1->setTransform(NXAffineTransform::identity);

            auto frame = view1->frame();
            frame.origin = Point(550, 69);
            view1->setFrame(frame);

        });
    });
}

void TestViewController::startRotate() {
//    UIView::animate(2, 0, UIViewAnimationOptions::curveLinear, [this]() {
//        rotationView->setTransform(NXAffineTransform::identity);
//    }, [this](bool res) {
//        startRotate();
//    });
    UIView::animate(2, 0, UIViewAnimationOptions::curveLinear, [this]() {
        rotationView->setTransform(NXAffineTransform::identity);
        rotationView->setTransform(NXAffineTransform::rotationBy(360 / 3));
    }, [this](bool res) {
        UIView::animate(2, 0, UIViewAnimationOptions::curveLinear, [this]() {
            rotationView->setTransform(NXAffineTransform::rotationBy(360 / 3 * 2));
        }, [this](bool res) {
            UIView::animate(2, 0, UIViewAnimationOptions::curveLinear, [this]() {
                rotationView->setTransform(NXAffineTransform::rotationBy(360));
            }, [this](bool res) {
                startRotate();
            });
        });
    });
}

//void TestViewController::touchesBegan(std::set<std::shared_ptr<UITouch>> touches, std::shared_ptr<UIEvent> event) {
//    UIViewController::touchesBegan(touches, event);
//
//    for (auto& touch: touches) {
//        if (!touch->view().expired()) {
//            printf("Touched %s\n", touch->view().lock()->tag.c_str());
//        }
//    }
//}
//
//void TestViewController::touchesMoved(std::set<std::shared_ptr<UITouch>> touches, std::shared_ptr<UIEvent> event) {
//    UIViewController::touchesMoved(touches, event);
//
//    for (auto& touch: touches) {
//        auto wview = touch->view();
//        if (wview.expired()) continue;
//
//        auto view = wview.lock();
//        if (view != view1) continue;
//
//        auto frame = view->frame();
//        auto touchDelta = touch->previousLocationIn(this->view()) - touch->locationIn(this->view());
//        frame.origin.x -= touchDelta.x;
//        frame.origin.y -= touchDelta.y;
//        view->setFrame(frame);
//    }
//}
