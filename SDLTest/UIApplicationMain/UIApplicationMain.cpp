//
//  UIApplicationMain.cpp
//  SDLTest
//
//  Created by Даниил Виноградов on 06.03.2023.
//

#include <UIApplicationMain/UIApplicationMain.hpp>
#include <UIApplication/UIApplication.hpp>
#include <UIRenderer/UIRenderer.hpp>

namespace UIKit {

void setupRenderAndRunLoop() {
    while (true) {
        auto currentTime = Timer();
        UIApplication::shared->handleEventsIfNeeded();
        auto window = UIApplication::shared->keyWindow.lock();
        if (window)
            UIRenderer::main()->render(window, currentTime);
    }
}

int UIApplicationMain(std::shared_ptr<UIApplicationDelegate> appDelegate) {
    auto application = std::make_shared<UIApplication>();
    UIApplication::shared = application;
    UIRenderer::_main = std::make_shared<UIRenderer>();

    application->delegate = appDelegate;

    if (!appDelegate->applicationDidFinishLaunchingWithOptions(application.get(), {})) {
        return 1;
    }

    setupRenderAndRunLoop();

    return 0;
};

}
