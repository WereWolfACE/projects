#pragma once

struct IFlashMovieClip;

#if defined(__clang__) || defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnon-virtual-dtor"
#endif

/**
* Используется в качестве callback для событий анимации в FlashMovieClip.
*/
struct IPlaybackOperation
{
	///Вызывается при построении каждого следующего кадра
	virtual void onFrameConstructed(IFlashMovieClip* movieClip, int frame) = 0;

	///Вызывается при окончании проигрывания loop'а, когда MovieClip готовится 
	///вновь перейти к первому кадру
	//Если внутри этой функции происходит управление клипом, то нужно вернуть false.
	//Если будет возвращено значение true, клип перейдет на первый кадр цикла.
	virtual bool onAnimationCompleted(IFlashMovieClip* movieClip) = 0;

	///Вызывается, когда MovieClip возвращается на первый кадр цикла
	virtual void onAnimationRestarted(IFlashMovieClip* movieClip) = 0;

	///Вызывается, когда MovieClip удаляется
	virtual void onMovieDisposed(IFlashMovieClip* movieClip) = 0;
};

#if defined(__clang__) || defined(__GNUC__)
#pragma GCC diagnostic pop
#endif
