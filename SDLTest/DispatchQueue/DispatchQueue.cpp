//
//  DispatchQueue.cpp
//  SDLTest
//
//  Created by Даниил Виноградов on 10.03.2023.
//

#include <DispatchQueue/DispatchQueue.hpp>

namespace UIKit {

DispatchQueue* DispatchQueue::_main = nullptr;

DispatchQueue* DispatchQueue::main() {
    if (!_main) _main = new DispatchQueue("main");
    return _main;
}

DispatchQueue::DispatchQueue(std::string tag): _tag(tag) { }

void DispatchQueue::async(std::function<void()> task) {
    _queue.push_back(task);
}

void DispatchQueue::performAll() {
    auto copy = _queue;
    _queue.clear();
    
    while (copy.size() > 0) {
        auto task = copy.back();
        copy.pop_back();
        task();
    }
}

}
