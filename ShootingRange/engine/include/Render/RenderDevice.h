#ifndef _RENDERDEVICE_H_
#define _RENDERDEVICE_H_

#include "Platform/TargetPlatforms.h"
#include "Render/RenderTypes.h"

#if defined( ENGINE_TARGET_WIN32 )
#	include "Render/RenderDeviceGLES2.h"
#elif defined( ENGINE_TARGET_IPHONE )
#	include "Render/RenderDeviceInterface.h"
#elif defined( ENGINE_TARGET_MACOS )
#	include "Render/RenderDeviceGLES2.h"
#elif defined( ENGINE_TARGET_LINUX )
#	include "Render/RenderDeviceInterface.h"
#else
#	error "Target platform isn't selected"
#endif

#endif //_RENDERDEVICE_H_
