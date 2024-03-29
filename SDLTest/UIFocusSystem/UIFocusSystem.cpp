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



    UIFocusUpdateContext context;
    context._previouslyFocusedItem = _focusedItem;
    context._focusHeading = makeFocusHeadingFromEvent(event);

    if (context._focusHeading == UIFocusHeading::none) return;

    std::weak_ptr<UIFocusItem> nextItem;
    if (focusedItem().expired()) nextItem = _rootWindow.lock()->searchForFocus();
    else {
        auto current = std::dynamic_pointer_cast<UIView>(focusedItem().lock());
        if (current->superview().expired()) {
            nextItem = _rootWindow.lock()->searchForFocus();
        } else {
            std::shared_ptr<UIFocusItem> potencianNextItem;
            while (true) {
                potencianNextItem = current->superview().lock()->getNextFocusItem(current, context._focusHeading);
                if (!potencianNextItem) { // if no next item to focus
                    if (!current->superview().lock()->superview().expired()) { // but item has parent, check parent
                        current = current->superview().lock();
                        continue;
                    } else { // else stop searching
                        break;
                    }
                }

                context._nextFocusedItem = potencianNextItem;

                bool currentIsFine = context.previouslyFocusedItem().expired() || context.previouslyFocusedItem().lock()->shouldUpdateFocusIn(context);
                bool nextIsFine = context.nextFocusedItem().expired() || context.nextFocusedItem().lock()->shouldUpdateFocusIn(context);

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
    auto item = _rootWindow.lock()->searchForFocus();

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


}
