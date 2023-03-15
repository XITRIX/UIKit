//
//  DispatchQueue.hpp
//  SDLTest
//
//  Created by Даниил Виноградов on 10.03.2023.
//

#pragma once

#include <queue>
#include <string>
#include <functional>
#include <thread>
#include <mutex>

#ifdef main
#undef main
#endif

namespace UIKit {

class DispatchQueue {
public:
    DispatchQueue(std::string tag);
    ~DispatchQueue();

    static DispatchQueue* main();
    static DispatchQueue* global();

    std::string tag() const { return _tag; }
    void async(std::function<void()> task);
private:
    static DispatchQueue* _main;
    static DispatchQueue* _global;
    std::queue<std::function<void()>> _queue;
    std::string _tag;

    pthread_t _task_loop_thread;
    std::mutex _m_async_mutex;
    volatile bool _task_loop_active = true;
    static void* task_loop(void* a);

    void performAll();

    friend void setupRenderAndRunLoop();
};

}
