//
//  UIApplication.cpp
//  SDLTest
//
//  Created by Даниил Виноградов on 08.02.2023.
//

#include <UIApplication/UIApplication.hpp>
#include <UIRenderer/UIRenderer.hpp>

namespace UIKit {

std::shared_ptr<UIApplication> UIApplication::shared = nullptr;

UIApplication::UIApplication() {
//    UIFont.loadSystemFonts();
}

void UIApplication::handleEventsIfNeeded() {
    auto e = SDL_Event();

    while (SDL_PollEvent(&e)) {
        switch (e.type) {
            case SDL_QUIT: {
                handleSDLQuit();
                return;
            }
            case SDL_CONTROLLERBUTTONDOWN: {
                if (e.cbutton.button == SDL_CONTROLLER_BUTTON_START) {
                    handleSDLQuit();
                }
                break;
            }
            case SDL_MOUSEBUTTONDOWN: {
//                let touch = UITouch(touchId: 0, at: .from(e.button), timestamp: e.timestampInSeconds)
//                let event = UIEvent(touch: touch)
//                sendEvent(event)
            }
            case SDL_MOUSEMOTION: {
//                if
//                    let event = UIEvent.activeEvents.first,
//                    let touch = event.allTouches?.first(where: { $0.touchId == 0 })
//                {
//                    let previousTimestamp = touch.timestamp
//                    let newTimestamp = e.timestampInSeconds
//
//                    touch.updateAbsoluteLocation(.from(e.motion))
//                    touch.timestamp = newTimestamp
//                    touch.phase = .moved
//
//                    // SDL adds timestamps on send which could be quite different to when the event actually occurred.
//                    // It's common to get two events with an unrealistically small time between them; don't send those.
//                    if (newTimestamp - previousTimestamp) > (5 / 1000) {
//                        sendEvent(event)
//                    }
//                }
            }
            case SDL_MOUSEBUTTONUP: {
//                if
//                    let event = UIEvent.activeEvents.first,
//                    let touch = event.allTouches?.first(where: { $0.touchId == 0 })
//                {
//                    touch.timestamp = e.timestampInSeconds
//                    touch.phase = .ended
//                    sendEvent(event)
//                }
            }
            case SDL_KEYUP: {
//#if DEBUG
//                let keyModifier = SDL_Keymod(UInt32(e.key.keysym.mod))
//                if keyModifier.contains(KMOD_LSHIFT) || keyModifier.contains(KMOD_RSHIFT) {
//                    switch e.key.keysym.sym {
//                    case 43, 61: // +/*, +/= keys. TODO send key events via UIEvent
//                        break
//                    case 45: break // -/_ key
//                    case 118: // "V"
//                        keyWindow?.printViewHierarchy()
//                    default:
//                        print(e.key.keysym.sym)
//                    }
//                }
//
//                if keyModifier.contains(KMOD_LGUI) || keyModifier.contains(KMOD_RGUI) {
//                    if e.key.keysym.sym == 114 { // CMD-R
//                        UIScreen.main = nil
//                        UIScreen.main = UIScreen()
//                    }
//                }
//#endif
//
//                let scancode = e.key.keysym.scancode
//                if scancode == .androidHardwareBackButton || scancode == .escapeKey {
//                    keyWindow?.deepestPresentedView().handleHardwareBackButtonPress()
//                }
            }
            case SDL_APP_WILLENTERBACKGROUND: {
                UIApplication::onWillEnterBackground();
            }
            case SDL_APP_DIDENTERBACKGROUND: {
                UIApplication::onDidEnterBackground();
            }
            case SDL_APP_WILLENTERFOREGROUND: {
                UIApplication::onWillEnterForeground();
            }
            case SDL_APP_DIDENTERFOREGROUND: {
                UIApplication::onDidEnterForeground();
            }
            case SDL_WINDOWEVENT: {
                switch (e.window.event)
                {
                    case SDL_WINDOWEVENT_RESIZED:
                        UIRenderer::main()->refreshScreenResolution(e.window.data1, e.window.data2);
                        break;
                    default:
                        break;
                }
                break;
            }
            default:
                break;
        }
    }
}

void UIApplication::handleSDLQuit() {
    if (delegate) delegate->applicationWillTerminate(this);
    UIApplication::shared = nullptr;
    SDL_Quit();
    exit(0);
}

void UIApplication::onWillEnterForeground() {
    UIApplication::shared->delegate->applicationDidEnterBackground(UIApplication::shared.get());
//    UIApplication.post(willEnterForegroundNotification)
}

void UIApplication::onDidEnterForeground() {
    UIApplication::shared->delegate->applicationDidBecomeActive(UIApplication::shared.get());
//    UIApplication.post(didBecomeActiveNotification)
}

void UIApplication::onWillEnterBackground() {
    UIApplication::shared->delegate->applicationWillResignActive(UIApplication::shared.get());
//    UIApplication.post(willResignActiveNotification)
}

void UIApplication::onDidEnterBackground() {
    UIApplication::shared->delegate->applicationDidEnterBackground(UIApplication::shared.get());
//    UIApplication.post(didEnterBackgroundNotification)
}

}
