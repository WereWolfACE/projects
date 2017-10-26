#ifndef _CORE_TIMER_H_
#define _CORE_TIMER_H_

#include "Platform/TargetPlatforms.h"

#ifdef ENGINE_TARGET_WIN32
	#define TimeType __int64

#elif defined(ENGINE_TARGET_IPHONE)
	#define TimeType uint64_t

#elif defined(ENGINE_TARGET_LINUX)
	#define TimeType uint64_t

#elif defined(ENGINE_TARGET_MACOS)
	#define TimeType uint64_t

#endif

namespace Core
{

// В однопоточном приложении дает возможность игре отрисоваться во время загрузок.
// Эта подозрительная функция используется только на винде,
// а у остальных пустая реализация.
void yield();

class Timer
{
	TimeType _startTime;
	TimeType _idleTicks;
	TimeType _idleStartTime;

public:
	Timer();

	void Start();

	void Pause();

	void Resume();

	float getElapsedTime(); ///< В секундах, нецелое.

public:
	static void Sync(); /// Инициализация таймера.

	static TimeType getTime(); ///< Absolute time в попугаях. / См. Interval() ниже.

	static float Interval(TimeType begin, TimeType end); ///< В секундах, нецелое.

	/// Задание одного ядра, на котором этот поток будет выполняться
	static void ChooseOneCore(bool &alreadySingleCoreMode);
};

} // end of namespace

#endif
