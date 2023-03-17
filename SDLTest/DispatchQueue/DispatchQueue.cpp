//
//  DispatchQueue.cpp
//  SDLTest
//
//  Created by Даниил Виноградов on 10.03.2023.
//

#include <DispatchQueue/DispatchQueue.hpp>
#include <Tools/Tools.hpp>
#include <libretro-common/retro_timers.h>

namespace UIKit {

std::shared_ptr<DispatchQueue> DispatchQueue::_main = nullptr;
std::shared_ptr<DispatchQueue> DispatchQueue::_global = nullptr;

std::shared_ptr<DispatchQueue> DispatchQueue::main() {
    if (!_main) _main = new_shared<DispatchQueue>("main");
    return _main;
}

std::shared_ptr<DispatchQueue> DispatchQueue::global() {
    if (!_global) _global = new_shared<DispatchQueue>("global");
    return _global;
}

void DispatchQueue::quit() {
    _main = nullptr;
    _global = nullptr;
}

DispatchQueue::DispatchQueue(std::string tag): _tag(tag) {
    if (tag != "main") {
        pthread_create(&_task_loop_thread, NULL, task_loop, this);
    }
}

DispatchQueue::~DispatchQueue() {
    _task_loop_active = false;
    if (_tag != "main") {
        pthread_join(_task_loop_thread, NULL);
    }
}

void DispatchQueue::async(std::function<void()> task) {
    std::lock_guard<std::mutex> guard(_m_async_mutex);
    _queue.push(task);
}

void DispatchQueue::performAll() {
    std::queue<std::function<void()>> copy;
    {
        std::lock_guard<std::mutex> guard(_m_async_mutex);
        std::swap( _queue, copy );
    }
    
    while (!copy.empty()) {
        auto task = copy.front();
        copy.pop();
        task();
    }
}

void* DispatchQueue::task_loop(void* a) {
    auto self = (DispatchQueue*) a;
    while (self->_task_loop_active) {
        self->performAll();
        retro_sleep(500);
    }
    return NULL;
}

}
