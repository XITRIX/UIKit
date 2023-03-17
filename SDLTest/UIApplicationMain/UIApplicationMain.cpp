//
//  UIApplicationMain.cpp
//  SDLTest
//
//  Created by Даниил Виноградов on 06.03.2023.
//

#include <UIApplicationMain/UIApplicationMain.hpp>
#include <UIApplication/UIApplication.hpp>
#include <UIRenderer/UIRenderer.hpp>
#include <DispatchQueue/DispatchQueue.hpp>

namespace UIKit {

void setupRenderAndRunLoop() {
    while (true) {
        auto currentTime = Timer();
        UIApplication::shared->handleEventsIfNeeded();
        if (!UIApplication::shared) break;
        DispatchQueue::main()->performAll(); // TODO: May be need to be after rendering loop
        auto window = UIApplication::shared->keyWindow;
        if (window.expired()) break;
        UIRenderer::main()->render(window.lock(), currentTime);
    }
}

int UIApplicationMain(std::shared_ptr<UIApplicationDelegate> appDelegate) {
    auto application = new_shared<UIApplication>();
    UIApplication::shared = application;
    UIRenderer::_main = new_shared<UIRenderer>();

    application->delegate = appDelegate;

    appDelegate->applicationNeedsXIBRegistration(application.get());
    if (!appDelegate->applicationDidFinishLaunchingWithOptions(application.get(), {})) {
        return 1;
    }

    setupRenderAndRunLoop();

    DispatchQueue::quit();
    UIApplication::shared = nullptr;
    UIRenderer::_main = nullptr;
    application = nullptr;
    appDelegate = nullptr;
    
    return 0;
};

}
