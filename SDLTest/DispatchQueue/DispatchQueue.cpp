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
    _queue.push(task);
}

void DispatchQueue::performAll() {
    std::queue<std::function<void()>> copy;
    std::swap( _queue, copy );
    
    while (!copy.empty()) {
        auto task = copy.front();
        copy.pop();
        task();
    }
}

}
