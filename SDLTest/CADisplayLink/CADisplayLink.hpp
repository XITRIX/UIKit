//
// Created by Daniil Vinogradov on 12/01/2024.
//

#pragma once

#include <vector>
#include <functional>
#include <Tools/SharedBase.hpp>

namespace UIKit {

    class CADisplayLink {
    public:
        explicit CADisplayLink(std::function<void()> func);
        ~CADisplayLink();

        void invalidate();
    private:
        bool isRunning = true;
        std::function<void()> func;
        static std::vector<CADisplayLink*> activeLinks;
        friend class DispatchQueue;
    };

}