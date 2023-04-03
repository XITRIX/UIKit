//
//  Platform.cpp
//  SDLTest-iOS
//
//  Created by Даниил Виноградов on 20.03.2023.
//

#include <Platform/Platform.hpp>

#import <UIKit/UIKit.h>

namespace UIKit {

UIEdgeInsets Platform::getPlatfromSafeArea() {
    auto var = [[[UIApplication sharedApplication]keyWindow] safeAreaInsets];
    return UIEdgeInsets(var.top, var.left, var.bottom, var.right);
}

}
