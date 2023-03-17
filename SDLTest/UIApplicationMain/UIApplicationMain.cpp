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
    UIApplication::shared = new_shared<UIApplication>();
    UIRenderer::_main = new_shared<UIRenderer>();

    UIApplication::shared->delegate = appDelegate;

    appDelegate->applicationNeedsXIBRegistration(UIApplication::shared.get());
    if (!appDelegate->applicationDidFinishLaunchingWithOptions(UIApplication::shared.get(), {})) {
        return 1;
    }

    setupRenderAndRunLoop();

    UIApplication::shared = nullptr;
    UIRenderer::_main = nullptr;
    
    return 0;
};

}
