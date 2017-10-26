/*
 *  AudioSession.h
 *  Engine
 *
 *  Created by Slava on 2/10/11.
 *  Copyright 2011 Playrix Entertainment. All rights reserved.
 *
 */

#ifndef AUDIO_SESSION_H_INCLUDED
#define AUDIO_SESSION_H_INCLUDED

#import <AudioToolbox/AudioToolbox.h>

namespace MM {
	
//
// Класс, инкапсулирующий работу с аудио-сессией в iOS.
// Все методы статические. Метод MM::AudioSession::Init() нужно
// вызывать во время загрузки приложения (applicationDidFinishLaunching).
// Всю остальную работу (перехват звуковых прерываний, и
// корректную их обработку) делает сам.
//
class AudioSession {
public:
	
	// Инициализировать сессию
	// Вызывать в applicationDidFinishLaunching
	static void Init();

	static bool IsActive();

	static bool TrySetActive();

	// Играется ли какой-то другой звук в устройстве
	// (проверять после загрузки gameInfo и, если true, 
	// устанавливать звук нашей фоновой музыки в ноль)
	static bool IsOtherAudioPlaying();
	
	// Выбрать категорию звукового приложения. Выделено в отдельную функцию,
	// т.к. это нужно делать в Init() и OnEndInterruption();
	static void ChooseAppropriateCategory();
	
private:
	
	// Статическая функция, которая регистрируется в Init() как callback для аудио-прерываний
	static void InterruptionListener(void * /*clientData*/, UInt32 interruptionState);
	
	// Реакция на начало прерывания
	// (во время звонков, будильников и т. п.)
	static void OnBeginInterruption();
	
	// Реакция на конец прерывания
	static void OnEndInterruption();
	
};

} // namespace MM
#endif