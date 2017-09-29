#ifndef _IOS_CLASSES_COMMON_H_
#define _IOS_CLASSES_COMMON_H_

// тут нужно располагать код, общий для ios и мака (NS...)


#ifdef __OBJC__

#ifndef PRODUCTION
#define NSLogD NSLog
#else
#define NSLogD(...)
#endif

#import <Foundation/Foundation.h>

NSString* UrlEsc(NSString *url);
NSString* MessageFromNSError(NSError* error);

#endif // __OBJC__

#ifdef __cplusplus

#include <string>

namespace utils
{

std::string GetFirstSupportedLang();
std::string GetFromSysctl(const std::string &param);
std::string GetDeviceID();
long getProcSize();
std::string UrlEsc(const std::string &url);
std::string GetBuildVersion();
}

#endif // __cplusplus


#endif // _IOS_CLASSES_COMMON_H_
