// Platform specific staff

#ifndef _PLATFORM_H_
#define _PLATFORM_H_

#include "Platform/TargetPlatforms.h"

#ifdef ENGINE_TARGET_WIN32

#include "Platform/win.h"

#else // !ENGINE_TARGET_WIN32

#define MAX_PATH 512
#define __forceinline

int strcpy_s(char* Dst, size_t DstSize, const char* Src);
int strcat_s(char* Dst, size_t DstSize, const char* Src);
int memcpy_s(void* Dst, size_t DstSize, const void* Src, size_t MaxCount);
int memmove_s(void* Dst, size_t DstSize, const void* Src, size_t MaxCount);
int sscanf_s(const char* Src, const char* Format, ...);
int sprintf_s(char* DstBuf, size_t DstSize, const char* Format, ...);

#endif // ENGINE_TARGET_*

#ifdef ENGINE_TARGET_IPHONE
#include "Platform/iphone.h"
#import <ios/Classes/Common.h>
#endif

#ifdef ENGINE_TARGET_MACOS
#include "Platform/mac.h"
#import <ios/Classes/Common.h>
#endif

#ifdef ENGINE_TARGET_LINUX
#include "../android/jni/android/android.h"
#include <stdint.h>
#endif

#endif // _PLATFORM_H_
