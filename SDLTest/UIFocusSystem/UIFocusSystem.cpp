//
//  UIFocusSystem.cpp
//  SDLTest
//
//  Created by Даниил Виноградов on 02.04.2023.
//

#include <UIFocusSystem/UIFocusSystem.hpp>
#include <UIPressesEvent/UIPressesEvent.hpp>
#include <UIPress/UIPress.hpp>
#include <UIWindow/UIWindow.hpp>

namespace UIKit {

UIFocusSystem::UIFocusSystem() { }

void UIFocusSystem::sendEvent(std::shared_ptr<UIEvent> event) {
    printf("Key pressed\n");



    UIFocusUpdateContext context;
    context._previouslyFocusedItem = _focusedItem;
    context._focusHeading = makeFocusHeadingFromEvent(event);

    if (context._focusHeading == UIFocusHeading::none) return;

    std::weak_ptr<UIFocusItem> nextItem;
    if (focusedItem().expired()) nextItem = searchForFocusInView(_rootWindow.lock());
    else {
        auto current = std::dynamic_pointer_cast<UIView>(focusedItem().lock());
        if (current->superview().expired()) {
            nextItem = searchForFocusInView(_rootWindow.lock());
        } else {
            std::shared_ptr<UIView> potencianNextItem = current;
            while (potencianNextItem) {
                potencianNextItem = current->superview().lock()->getNextFocusItem(potencianNextItem, context._focusHeading);
                context._nextFocusedItem = potencianNextItem;

                bool currentIsFine = !current || current->shouldUpdateFocusIn(context);
                bool nextIsFine = !potencianNextItem || potencianNextItem->shouldUpdateFocusIn(context);

                if (currentIsFine && nextIsFine) { break; }
            }
            nextItem = potencianNextItem;
        }
    }

    if (nextItem.expired()) return;

    context._nextFocusedItem = nextItem;
    applyFocusToItem(nextItem.lock(), context);
}

void UIFocusSystem::updateFocus() {
    auto item = searchForFocusInView(_rootWindow.lock());

    UIFocusUpdateContext context;
    context._previouslyFocusedItem = _focusedItem;
    context._nextFocusedItem = item;
    context._focusHeading = UIFocusHeading::none;

    applyFocusToItem(item, context);
}

std::shared_ptr<UIFocusItem> UIFocusSystem::findItemToFocus() {
    auto vc = _rootWindow.lock()->rootViewController();
    while (!vc->children().empty()) {
        vc = vc->children().front();
    }
    return vc->view();
}

void UIFocusSystem::applyFocusToItem(std::shared_ptr<UIFocusItem> item, UIFocusUpdateContext context) {
    UIFocusAnimationCoordinator coordinator;
    _focusedItem = item;

    if (!context.previouslyFocusedItem().expired()) { context.previouslyFocusedItem().lock()->didUpdateFocusIn(context, &coordinator); }
    if (!context.nextFocusedItem().expired()) { context.nextFocusedItem().lock()->didUpdateFocusIn(context, &coordinator); }

    UIFocusAnimationContext animationContext;
    UIView::animate(animationContext.duration(), 0, curveEaseOut, [context, coordinator, animationContext]() {
        for (auto animation: coordinator._coordinatedAnimations) { animation(); }
        for (auto animation: coordinator._coordinatedFocusingAnimations) { animation(animationContext); }
        for (auto animation: coordinator._coordinatedUnfocusingAnimations) { animation(animationContext); }
    }, [coordinator](bool res) {
        for (auto completion: coordinator._coordinatedAnimationCompletions) { completion(); }
    });
}

UIFocusHeading UIFocusSystem::makeFocusHeadingFromEvent(std::shared_ptr<UIEvent> event) {
    auto pevent = std::dynamic_pointer_cast<UIPressesEvent>(event);
    if (pevent == nullptr) return UIFocusHeading::none;

    for (auto press: pevent->allPresses()) {
        if (!press->key().has_value()) continue;
        if (press->phase() != UIPressPhase::began) continue;

        auto keyCode = press->key().value().keyCode();

        if (keyCode == UIKeyboardHIDUsage::keyboardRightArrow) {
            return UIFocusHeading::right;
        }

        if (keyCode == UIKeyboardHIDUsage::keyboardUpArrow) {
            return UIFocusHeading::up;
        }

        if (keyCode == UIKeyboardHIDUsage::keyboardLeftArrow) {
            return UIFocusHeading::left;
        }

        if (keyCode == UIKeyboardHIDUsage::keyboardDownArrow) {
            return UIFocusHeading::down;
        }
    }

    return UIFocusHeading::none;
}

std::shared_ptr<UIFocusItem> UIFocusSystem::searchForFocusInView(std::shared_ptr<UIView> view) {
    if (view->canBecomeFocused()) { return view; }

    for (auto& child: view->subviews()) {
        auto res = searchForFocusInView(child);
        if (res) return res;
    }

    return nullptr;
}


}
