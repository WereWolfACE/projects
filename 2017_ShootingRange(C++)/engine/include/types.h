#ifndef _TYPES_H_
#define _TYPES_H_

#include <cstdint>
#include <cstdio>
#include <cmath>

#include <istream>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <deque>
#include <string>
#include <algorithm>
#include <stdexcept>
#include <limits>
#include <typeindex>

// Disable shadow warnings in libs
#if defined(__clang__) || defined(__llvm__)
#pragma clang diagnostic ignored "-Wshadow"
#endif

#include "Platform/TargetPlatforms.h"

#ifdef ENGINE_TARGET_WIN32
#	define WIN32_LEAN_AND_MEAN
#	define NOMINMAX
#	include <windows.h>

#	if defined(_DEBUG)
#		define _CRTDBG_MAP_ALLOC
#		include <stdlib.h>
#		include <crtdbg.h>
#	endif
#endif // ENGINE_TARGET_WIN32

// на MacOS отключаем системный макрос check, иначе будет конфликт с luabind и boost
#if defined(ENGINE_TARGET_MACOS)
#if defined(check)
#undef check
#endif // check
#endif

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wconversion"
#endif

#include <boost/smart_ptr.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/config.hpp>

#if defined(__clang__)
#pragma clang diagnostic pop
#endif

#include <pugixml.hpp>

#define RAPIDXML_STATIC_POOL_SIZE 10 * 1024
#include "../rapidxml/rapidxml.hpp"

#include "EngineAssert.h"
#include "Core/Log.h"
#include "Core/KeyCodes.h"
#include "Core/CharCodes.h"

#ifdef ENGINE_TARGET_MACOS
#include <signal.h>
#include <sys/stat.h>
#include <mach/mach_time.h>
#include <unistd.h>
#include <glob.h>
#include <OpenGL/OpenGL.h>
#include <OpenGL/glu.h>
#include <AGL/agl.h>
#endif //ENGINE_TARGET_MACOS

#ifdef ENGINE_TARGET_LINUX
#   include <sys/stat.h>
#   include <sys/types.h>
#   include <errno.h>
#endif

#ifdef ENGINE_TARGET_ANDROID
#include <jni.h>
#include <android/log.h>
#include <GLES/gl.h>
#include <GLES/glext.h>
#endif // ENGINE_TARGET_ANDROID

#if defined(ENGINE_TARGET_IPHONE)
#include <sys/stat.h>
#endif // ENGINE_TARGET_IPHONE

// Disable some annoying warnings
//
#if defined(_MSC_VER)
// warning C4244: conversion from 'float' to 'int', possible loss of data
// warning C4800: forcing value to bool 'true' or 'false' (performance warning)
// warning C4305: truncation from 'double' to 'float'
#pragma warning(disable: 4244 4305 4800)
#elif defined(__clang__) || defined(__llvm__)
#pragma clang diagnostic ignored "-Wdouble-promotion"
#pragma clang diagnostic ignored "-Wfloat-conversion"
#pragma clang diagnostic ignored "-Wunused"
#pragma clang diagnostic ignored "-Wshadow"
#pragma clang diagnostic ignored "-Wparentheses"
#else
#pragma GCC diagnostic ignored "-Wdouble-promotion"
//#pragma GCC diagnostic ignored "-Wfloat-conversion"
#pragma GCC diagnostic ignored "-Wunused"
#pragma GCC diagnostic ignored "-Wshadow"
#pragma GCC diagnostic ignored "-Wparentheses"
#endif

#endif //_TYPES_H_
