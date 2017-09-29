#ifndef __THREADSUPPORT_H__
#define __THREADSUPPORT_H__

#pragma once

#if !defined(ENGINE_TARGET_WINDOWS_METRO)//на WinRT собственные треды
#include <boost/thread.hpp>
#include <boost/thread/recursive_mutex.hpp>

typedef boost::recursive_mutex MUTEX_TYPE;
typedef boost::recursive_mutex::scoped_lock MUTEX_LOCK_TYPE;
#endif

#if defined(ENGINE_TARGET_WINDOWS_METRO)
#include "Core/CriticalSection.h"
#include "Core/CriticalSectionLock.h"

typedef CriticalSection MUTEX_TYPE;
typedef CriticalSectionLock MUTEX_LOCK_TYPE;
#endif

#endif // __THREADSUPPORT_H__
