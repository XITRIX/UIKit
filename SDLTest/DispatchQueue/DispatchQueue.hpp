//
//  DispatchQueue.hpp
//  SDLTest
//
//  Created by Даниил Виноградов on 10.03.2023.
//

#pragma once

#include <vector>
#include <string>
#include <functional>

#ifdef main
#undef main
#endif

namespace UIKit {

class DispatchQueue {
public:
    DispatchQueue(std::string tag);
    static DispatchQueue* main();

    std::string tag() const { return _tag; }
    void async(std::function<void()> task);
private:
    static DispatchQueue* _main;
    std::vector<std::function<void()>> _queue;
    std::string _tag;

    void performAll();

    friend void setupRenderAndRunLoop();
};

}
