// унифицированные #define для поддерживаемых платформ

#ifndef _PLATFORM_TARGETPLATFORMS_H_
#define _PLATFORM_TARGETPLATFORMS_H_

/////////////////////////////////////////////////////////////////////////////////
// Microsoft
/////////////////////////////////////////////////////////////////////////////////

#if defined(_WIN32) || defined(WIN32)

	#define ENGINE_TARGET_WIN32

	#if defined(WINAPI_FAMILY) && WINAPI_FAMILY == WINAPI_PARTITION_APP

		#define ENGINE_TARGET_WINDOWS_METRO
		#define ENGINE_TARGET_WIN32_DX11
		
	#endif

#endif

/////////////////////////////////////////////////////////////////////////////////
// Apple
/////////////////////////////////////////////////////////////////////////////////

#if defined(__APPLE__)

#include "TargetConditionals.h"

#if defined(TARGET_OS_IPHONE) && TARGET_OS_IPHONE

	// iPhone (настоящий или симулятор)
	#define ENGINE_TARGET_IPHONE

	#if defined(TARGET_IPHONE_SIMULATOR) && TARGET_IPHONE_SIMULATOR

		// iPhone Simulator
		#define ENGINE_TARGET_IPHONE_SIMULATOR
	
	#else

		// iPhone OS
		#define ENGINE_TARGET_IPHONEOS

	#endif

#else

	// Mac OS
	#define ENGINE_TARGET_MACOS

#endif

#endif


/////////////////////////////////////////////////////////////////////////////////
// Android
/////////////////////////////////////////////////////////////////////////////////

#if defined (__ANDROID__) || defined (ANDROID)

	#define ENGINE_TARGET_ANDROID

#endif

/////////////////////////////////////////////////////////////////////////////////
// Linux
/////////////////////////////////////////////////////////////////////////////////

#if defined(__linux__)

	#define ENGINE_TARGET_LINUX

#endif

#endif // _PLATFORM_TARGETPLATFORMS_H_
