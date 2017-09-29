#ifndef _PLATFORM_IPHONE_H_
#define _PLATFORM_IPHONE_H_


#ifdef __OBJC__

#import <UIKit/UIKit.h>

UIViewController* getAppViewController();

#endif


#ifdef __cplusplus

#include <string>

namespace utils
{

bool IsIosVersionLessThan(const std::string& version);
bool IsPushNotificationsEnabled();

}

#endif


// Для universal app данная функция возвращает, является ли железяка айпадом или нет
// Для iphone приложений всегда false
// Для ipad приложений всегда true
bool DeviceIpad();
bool isRetina();

#endif
