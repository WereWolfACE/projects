#ifndef _PLATFORM_MAC_H_
#define _PLATFORM_MAC_H_

#ifdef __cplusplus

namespace utils
{
bool IsPushNotificationsEnabled();
}

#endif

#ifdef __OBJC__

#import <Foundation/Foundation.h>
#import <AppKit/AppKit.h>

namespace utils
{

NSData* GetPngRepresentationForImage(NSImage *image);
NSData* GetJpgRepresentationForImage(NSImage *image);

}

#endif

#endif // _PLATFORM_MAC_H_
