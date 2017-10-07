#ifndef _CORE_APPLICATION_H_
#define _CORE_APPLICATION_H_

#include "Core/App/ApplicationBase.h"

#ifdef ENGINE_TARGET_WIN32
#	include "Core/App/Win_Application.h"
#endif
#ifdef ENGINE_TARGET_IPHONE
#	include "Core/App/IOS_Application.h"
#endif
#ifdef ENGINE_TARGET_MACOS
#	include "Core/App/Mac_Application.h"
#endif
#ifdef ENGINE_TARGET_LINUX
#	include "Core/App/Android_Application.h"
#endif

#endif //_CORE_APPLICATION_H_
