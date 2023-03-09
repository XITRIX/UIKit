//
//  UIApplication.cpp
//  SDLTest
//
//  Created by Даниил Виноградов on 08.02.2023.
//

#include <UIApplication/UIApplication.hpp>
#include <UIRenderer/UIRenderer.hpp>
#include <UITouch/UITouch.hpp>
#include <Utils/Utils.hpp>

namespace UIKit {

std::shared_ptr<UIApplication> UIApplication::shared = nullptr;

UIApplication::UIApplication() {
    // TODO: Replace with Bunbles
#ifdef __SWITCH__
    Utils::resourcePath = "romfs:/";
#elif __APPLE__
#include <TargetConditionals.h>
#if TARGET_IPHONE_SIMULATOR || TARGET_OS_IPHONE
    Utils::resourcePath = std::string(SDL_GetBasePath()) + "/assets/";
#endif
#endif

//    UIFont.loadSystemFonts();
}

void UIApplication::handleEventsIfNeeded() {
    auto e = SDL_Event();

    while (SDL_PollEvent(&e)) {
        handleSDLEvent(e);
    }
}


void UIApplication::handleSDLEvent(SDL_Event e) {
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
        case SDL_FINGERDOWN: {
            auto renderSize = UIRenderer::_main->bounds().size;
            auto fingerPoint = Point(renderSize.width * e.tfinger.x, renderSize.height * e.tfinger.y);
//                printf("Touch id: %lld Begin, X:%f - Y:%f\n", e.tfinger.fingerId, fingerPoint.x, fingerPoint.y);
            auto touch = std::make_shared<UITouch>(e.tfinger.fingerId, fingerPoint, getCPUTimeUsec());
            auto event = std::shared_ptr<UIEvent>(new UIEvent(touch));
            UIEvent::activeEvents.push_back(event);
            sendEvent(event);
            break;
        }
        case SDL_FINGERMOTION: {
            auto renderSize = UIRenderer::_main->bounds().size;
            auto fingerPoint = Point(renderSize.width * e.tfinger.x, renderSize.height * e.tfinger.y);
//                printf("Touch id: %lld Moved, X:%f - Y:%f\n", e.tfinger.fingerId, fingerPoint.x, fingerPoint.y);

            std::shared_ptr<UIEvent> event;
            std::shared_ptr<UITouch> touch;

            for (auto& levent: UIEvent::activeEvents) {
                for (auto& ltouch: levent->allTouches()) {
                    if (ltouch->touchId() == e.tfinger.fingerId) {
                        event = levent;
                        touch = ltouch;
                    }
                }
            }

            if (!event || !touch) return;

            auto previousTimestamp = touch->timestamp();
            auto newTimestamp = getCPUTimeUsec();

            touch->updateAbsoluteLocation(fingerPoint);
            touch->_timestamp = newTimestamp;
            touch->_phase = UITouchPhase::moved;

            // SDL adds timestamps on send which could be quite different to when the event actually occurred.
            // It's common to get two events with an unrealistically small time between them; don't send those.
            if ((newTimestamp - previousTimestamp) > (5 / 1000)) {
                sendEvent(event);
            }

            break;
        }
        case SDL_FINGERUP: {
//                printf("Touch id: %lld Ended\n", e.tfinger.fingerId);

            std::shared_ptr<UIEvent> event;
            std::shared_ptr<UITouch> touch;

            for (auto& levent: UIEvent::activeEvents) {
                for (auto& ltouch: levent->allTouches()) {
                    if (ltouch->touchId() == e.tfinger.fingerId) {
                        event = levent;
                        touch = ltouch;
                    }
                }
            }

            if (!event || !touch) return;

            touch->_timestamp = getCPUTimeUsec();
            touch->_phase = UITouchPhase::ended;

            sendEvent(event);
            UIEvent::activeEvents.erase(std::remove(UIEvent::activeEvents.begin(), UIEvent::activeEvents.end(), event), UIEvent::activeEvents.end());

            break;
        }
        case SDL_MOUSEBUTTONDOWN: {
//                printf("Mouse click Begin, X:%d - Y:%d\n", e.button.x, e.button.y);
//                auto touch = std::make_shared<UITouch>(0, Point(e.button.x, e.button.y), getCPUTimeUsec());
//                auto event = std::shared_ptr<UIEvent>(new UIEvent(touch));
//                sendEvent(event);
//                break;
        }
        case SDL_MOUSEMOTION: {
//                printf("Mouse click Move, X:%d - Y:%d\n", e.button.x, e.button.y);

//                auto event = UIEvent::activeEvents;
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
            break;
        }
        case SDL_MOUSEBUTTONUP: {
//                printf("Mouse click Ended, X:%d - Y:%d\n", e.button.x, e.button.y);
//                if
//                    let event = UIEvent.activeEvents.first,
//                    let touch = event.allTouches?.first(where: { $0.touchId == 0 })
//                {
//                    touch.timestamp = e.timestampInSeconds
//                    touch.phase = .ended
//                    sendEvent(event)
//                }
            break;
        }
        case SDL_KEYDOWN: {
            if (e.key.keysym.sym == SDLK_q) {
                handleSDLQuit();
            }
            break;
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
            break;
        }
        case SDL_APP_DIDENTERBACKGROUND: {
            UIApplication::onDidEnterBackground();
            break;
        }
        case SDL_APP_WILLENTERFOREGROUND: {
            UIApplication::onWillEnterForeground();
            break;
        }
        case SDL_APP_DIDENTERFOREGROUND: {
            UIApplication::onDidEnterForeground();
            break;
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

void UIApplication::handleSDLQuit() {
    if (delegate) delegate->applicationWillTerminate(this);
    UIApplication::shared = nullptr;
    SDL_Quit();
    exit(0);
}

void UIApplication::sendEvent(std::shared_ptr<UIEvent> event) {
    for (auto& touch: event->allTouches()) {
        touch->_window = keyWindow;
    }

    if (!keyWindow.expired())
        keyWindow.lock()->sendEvent(event);
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
