//
//  UIFocusEnvironment.hpp
//  SDLTest
//
//  Created by Даниил Виноградов on 02.04.2023.
//

#pragma once

#include <UIFocusAnimationCoordinator/UIFocusAnimationCoordinator.hpp>
#include <memory>
#include <vector>

namespace UIKit {

enum class UIFocusHeading {
    up,
    down,
    left,
    right,
    next,
    previous,
    first,
    last
};

class UIFocusUpdateContext;
class UIFocusEnvironment {
public:

    /// The preferred focus environments define where to search for the default focused item in an environment, such as when focus updates programmatically.
    /// Starting from the target environment, each preferred focus environment is recursively searched in the order of the array until an eligible, focusable item is found.
    /// Preferred focus environments can include focusable and non-focusable items, in addition to non-item environments. Returning an empty array is equivalent to returning an array containing only 'self'.
    std::vector<std::shared_ptr<UIFocusEnvironment>> preferredFocusEnvironments() { return _preferredFocusEnvironments; }

    /// The parent focus environment of this environment, or nil if no parent exists.
    /// NOTE: If you implement this method, you must return a non-nil value for parent focus environment, otherwise your focus environment will not participate in focus interactions.
    virtual std::shared_ptr<UIFocusEnvironment> parentFocusEnvironment() = 0;

    /// Asks whether the system should allow a focus update to occur.
    virtual bool shouldUpdateFocusIn(UIFocusUpdateContext context);

    virtual void didUpdateFocusIn(UIFocusUpdateContext context, UIFocusAnimationCoordinator coordinator);

private:
    std::vector<std::shared_ptr<UIFocusEnvironment>> _preferredFocusEnvironments;
};

class UIFocusItem: public UIFocusEnvironment {

    /// Indicates whether or not this item is currently allowed to become focused.
    /// Returning NO restricts the item from being focusable, even if it is visible in the user interface. For example, UIControls return NO if they are disabled.
    virtual bool canBecomeFocused() { return false; }

    virtual bool isFocused() { return false; }
};

class UIFocusUpdateContext {
public:
    /// The item that was focused before the update, i.e. where focus is updating from. May be nil if no item was focused, such as when focus is initially set.
    std::weak_ptr<UIFocusItem> previouslyFocusedItem() { return _previouslyFocusedItem; }

    /// The item that is focused after the update, i.e. where focus is updating to. May be nil if no item is being focused, meaning focus is being lost.
    std::weak_ptr<UIFocusItem> nextFocusedItem() { return _nextFocusedItem; }

    /// The focus heading in which the update is occurring.
    UIFocusHeading focusHeading() { return _focusHeading; }
private:
    std::weak_ptr<UIFocusItem> _previouslyFocusedItem;
    std::weak_ptr<UIFocusItem> _nextFocusedItem;
    UIFocusHeading _focusHeading;
};

}
