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
        for (auto recognizer : view->gestureRecognizers())
            recognizers.push_back(recognizer);
        view = view->superview().lock();
    }

    return recognizers;
}

UIWindow::UIWindow() {
    setBackgroundColor(UIColor::systemBackground);
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
//    guard
//        let allTouches = event.allTouches,
//        let currentTouch = allTouches.first,
//        let hitView = currentTouch.view ?? hitTest(currentTouch.location(in: nil), with: nil)
//    else { return }

    for (auto& currentTouch: event->allTouches()) {
        auto wHitView = currentTouch->view();
        if (wHitView.expired()) wHitView = hitTest(currentTouch->locationIn(nullptr), nullptr);
        if (wHitView.expired()) continue;
        auto hitView = wHitView.lock();

        switch (currentTouch->phase()) {
            case UITouchPhase::began: {
//                UIEvent::activeEvents.push_back(event);
                currentTouch->_view = hitView;
                currentTouch->_gestureRecognizers = getRecognizerHierachyFromView(hitView);

                currentTouch->runTouchActionOnRecognizerHierachy([](auto gestureRecognizer) {
//                gestureRecognizer->touchesBegan(allTouches, with: event);
                });

                if (!currentTouch->hasBeenCancelledByAGestureRecognizer()) {
                    hitView->touchesBegan(event->allTouches(), event);
                }
            }
            case UITouchPhase::moved: {
                currentTouch->runTouchActionOnRecognizerHierachy([](auto gestureRecognizer) {
//                gestureRecognizer->touchesMoved(allTouches, event);
                });
                if (!currentTouch->hasBeenCancelledByAGestureRecognizer()) {
                    hitView->touchesMoved(event->allTouches(), event);
                }
            }
            case UITouchPhase::ended: {
                // compute the value before ending the touch on the recognizer hierachy
                // otherwise `hasBeenCancelledByAGestureRecognizer` will be false because the state was reset already
                auto hasBeenCancelledByAGestureRecognizer = currentTouch->hasBeenCancelledByAGestureRecognizer();

                currentTouch->runTouchActionOnRecognizerHierachy([](auto gestureRecognizer) {
//                gestureRecognizer->touchesEnded(allTouches, with: event);
                });

                if (!hasBeenCancelledByAGestureRecognizer) {
                    hitView->touchesEnded(event->allTouches(), event);
                }

//                UIEvent::activeEvents.erase(event);
            }
        }
    }


}

}
