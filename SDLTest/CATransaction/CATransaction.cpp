//
//  CATransaction.cpp
//  SDLTest
//
//  Created by Даниил Виноградов on 04.03.2023.
//

#include <CATransaction/CATransaction.hpp>

namespace UIKit {

std::vector<CATransaction> CATransaction::transactionStack;

void CATransaction::begin() {
    transactionStack.push_back(CATransaction());
}

void CATransaction::commit() {
    transactionStack.pop_back();
}

bool CATransaction::disableActions() {
    if (transactionStack.empty()) return false;
    return transactionStack.back().disableActions();
}

void CATransaction::setDisableActions(bool newValue) {
    if (transactionStack.empty()) { return; }
    transactionStack[transactionStack.size() - 1].disableActions_ = newValue;
}

float CATransaction::animationDuration() {
    if (transactionStack.empty()) return CALayer::defaultAnimationDuration;
    return transactionStack.back().animationDuration_;
}

void CATransaction::setAnimationDuration(float newValue) {
    if (transactionStack.empty()) { return; }
    transactionStack[transactionStack.size() - 1].animationDuration_ = newValue;
}

}
