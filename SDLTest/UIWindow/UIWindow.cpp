//
//  UIWindow.cpp
//  SDLTest
//
//  Created by Даниил Виноградов on 06.03.2023.
//

#include <UIWindow/UIWindow.hpp>
#include <UIApplication/UIApplication.hpp>
#include <UIRenderer/UIRenderer.hpp>
#include <UITouch/UITouch.hpp>

namespace UIKit {

std::vector<std::weak_ptr<UIGestureRecognizer>> getRecognizerHierachyFromView(std::shared_ptr<UIView> view) {
    std::vector<std::weak_ptr<UIGestureRecognizer>> recognizers;
    while (view) {
        for (auto recognizer : *view->gestureRecognizers())
            recognizers.push_back(recognizer);
        view = view->superview().lock();
    }

    return recognizers;
}

UIWindow::UIWindow() {
    setBackgroundColor(UIColor::systemBackground);
    yoga->setEnabled(false);
}

std::shared_ptr<UIWindow> UIWindow::window() {
    return shared_from_base<UIWindow>();
}

void UIWindow::setRootViewController(std::shared_ptr<UIViewController> rootViewController) {
    _rootViewController = rootViewController;
}

void UIWindow::makeKeyAndVisible() {
//    self.safeAreaInsets = UIWindow.getSafeAreaInsets()
    auto window = std::static_pointer_cast<UIWindow>(shared_from_this());
    window->setBounds(UIRenderer::main()->bounds());
    UIApplication::shared->keyWindow = window;

    auto viewController = _rootViewController;
    if (viewController) {
        viewController->loadViewIfNeeded();
        viewController->view()->setFrame(this->bounds());
        viewController->viewWillAppear(false);
        addSubview(viewController->view());
        viewController->viewDidAppear(false);
    }
}

void UIWindow::sendEvent(std::shared_ptr<UIEvent> event) {
    for (auto& touch: event->allTouches()) {
        auto wHitView = touch->view();
        if (wHitView.expired()) wHitView = hitTest(touch->locationIn(nullptr), nullptr);
        if (wHitView.expired()) continue;
        auto hitView = wHitView.lock();

        switch (touch->phase()) {
            case UITouchPhase::began: {
                touch->_view = hitView;
                touch->_gestureRecognizers = getRecognizerHierachyFromView(hitView);

                touch->runTouchActionOnRecognizerHierachy([touch, event](auto gestureRecognizer) {
                    gestureRecognizer->touchesBegan({ touch }, event);
                });

                if (!touch->hasBeenCancelledByAGestureRecognizer()) {
                    hitView->touchesBegan(event->allTouches(), event);
                }
                break;
            }
            case UITouchPhase::moved: {
                touch->runTouchActionOnRecognizerHierachy([touch, event](auto gestureRecognizer) {
                    gestureRecognizer->touchesMoved({ touch }, event);
                });
                if (!touch->hasBeenCancelledByAGestureRecognizer()) {
                    hitView->touchesMoved(event->allTouches(), event);
                }
                break;
            }
            case UITouchPhase::ended: {
                // compute the value before ending the touch on the recognizer hierachy
                // otherwise `hasBeenCancelledByAGestureRecognizer` will be false because the state was reset already
                auto hasBeenCancelledByAGestureRecognizer = touch->hasBeenCancelledByAGestureRecognizer();

                touch->runTouchActionOnRecognizerHierachy([touch, event](auto gestureRecognizer) {
                    gestureRecognizer->touchesEnded({ touch }, event);
                });

                if (!hasBeenCancelledByAGestureRecognizer) {
                    hitView->touchesEnded(event->allTouches(), event);
                }
                break;
            }
        }
    }
}

void UIWindow::layoutSubviews() {
    UIView::layoutSubviews();

    if (_rootViewController) {
        _rootViewController->view()->setFrame(this->bounds());
    }
}

}
