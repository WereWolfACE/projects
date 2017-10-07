#pragma once

#include <string>
#include <Core/Log.h>
#include <stdexcept>

#define USE_MARKSWEEP_GC //используется сборщик мусора
#define GC_CUSTOM_FINALIZER //используется GC_FINALIZER
#define WRITE_LOG //ведется лог

#if defined(WRITE_LOG)
#define FLASH_INFO(msg) Log::Info(msg)
#define FLASH_WARNING(msg) Log::Warn(msg)
#define FLASH_ERROR(msg) Log::Error(msg)
#else
#define FLASH_INFO(msg)
#define FLASH_WARNING(msg)
#define FLASH_ERROR(msg)
#endif
