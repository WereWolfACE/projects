#pragma once

#include "IFlashSprite.h"
#include "IPlaybackOperation.h"

#if defined(__clang__) || defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnon-virtual-dtor"
#endif

struct IFlashMovieClip :  public IFlashSprite
{
	virtual void reset() = 0;

	virtual float getFps() = 0;

	/**
	* Выполняет переход к следующему кадру на шкале времени
	*/
	virtual int nextFrame() = 0;

	virtual int prevFrame() = 0;

	/**
	* Выполняет переход к произвольному кадру.
	* При этом выполняется переход на ближайший левый ключевой кадр и последовательное
	* декодирование кадров до заданного.
	* Для большинства случаев, более эффективным способом будет переход на метку
	*/
	virtual int gotoFrame(int frameId) = 0;
	/**
	* Выполняет переход к помеченому кадру.
	* Эффективная операция, т.к. все помеченые кадры являются ключевыми
	*/
	virtual int gotoLabel(const std::string& label) = 0;
	/**
	* Возвращает текущий отображаемый кадр
	*/
	virtual int getCurrentFrame() = 0;

	/**
	* Управляет воспроизведением клипа
	*/
	virtual void setPlayback(bool value) = 0;


	/**
	* Устанавливает режим проигрывания по кольцу подпоследовательности кадров.
	*/
	virtual void setLooping(bool isLooping) = 0;
	/**
	* Устанавливает подпоследовательность кадров для цикла.
	* @param startFrame задает первый кадр последовательности
	* @param endFrame задает последний кадр, входящий в последовательность
	*/
	virtual void setLoop(int startFrame, int endFrame) = 0;

	/**
	* Возвращает кадр, соответствующий заданной метке.
	* Метка должна существовать.
	*/
	virtual int resolveLabelFrame(const std::string& label) = 0;

	/**
	* Устанавливает интерфейс управления воспроизведением клипа
	*/
	virtual void setPlaybackOperation(IPlaybackOperation* playbackOperation) = 0;

	virtual int countFrames() = 0;
};

#if defined(__clang__) || defined(__GNUC__)
#pragma GCC diagnostic pop
#endif
