#ifndef _CORE_WINDOW_H_
#define _CORE_WINDOW_H_

#ifdef ENGINE_TARGET_WIN32
#	include "Core/App/Win_Window.h"
#endif
#ifdef ENGINE_TARGET_MACOS
#	include "Core/App/Mac_Window.h"
#endif

#endif // _CORE_WINDOW_H_
