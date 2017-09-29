#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

#include "Core/App/EngineAppDelegate.h"

@interface MainViewController : UIViewController {
}

@end

NSUInteger ScreenOrientationToUIInterfaceOrientationMask(ScreenOrientation screenOrientation);
ScreenOrientation UIInterfaceOrientationToScreenOrientation(UIInterfaceOrientation interfaceOrientation);
