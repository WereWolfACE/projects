//
// Created by playrix on 31.07.15.
// Copyright (c) 2015 Playrix Entertainment. All rights reserved.
//

#ifndef __KeyboardManager_H_
#define __KeyboardManager_H_
#include "Utils/FRect.h"
#include "Utils/Math.hpp"

// ---------------------------------------------------------------------------------------------------------------------
// KeyboardManager - класс для управления клавиатурой iOS
//
// Для того, чтобы начать получать события клавиатуры, класс должен быть отнаследован
// от интерфейса IKeyboardNotificationReceiver и зарегистрироваться как получатель,
// вызвав KeyboardManager::Instance().RegisterReceiver()
// ---------------------------------------------------------------------------------------------------------------------



// Интерфейс для классов, желающих получать события клавиатуры
class IKeyboardNotificationReceiver
{
public:
	// Вызывается, когда клавиатура полностью показана
	virtual void OnKeyboardShown() {}
	// Вызывается, когда клавиатура полностью скрыта
	virtual void OnKeyboardHidden() {}
	// Вызывается, когда клавиатура начала изменять свое положение/размер
	virtual void OnKeyboardBeginChangeFrame(const FRect& begin_frame, const FRect& end_frame, float duration) {}
	// Вызывается, когда клавиатура закончила изменять свое положение/размер
	virtual void OnKeyboardEndChangeFrame() {}
	// Вызывается, когда клавиатура начала скрываться
	virtual void OnKeyboardWillHide() {}
};

class KeyboardManager
{

public:
	static KeyboardManager& Instance() {
		static KeyboardManager instance;
		return instance;
	}
	
private:
	KeyboardManager() {}
	~KeyboardManager(){};
	KeyboardManager( const KeyboardManager& );
	KeyboardManager& operator=( KeyboardManager& );

public:
	
	void RegisterReceiver(IKeyboardNotificationReceiver* receiver);
	void RemoveReceiver(IKeyboardNotificationReceiver* receiver);
	
	void ShowNumericKeyboard();
	void ShowKeyboard();
	void HideKeyboard();
	
	bool IsKeyboardActive() const;
	bool IsKeyboardVisible() const;
	
	void OnKeyboardShown();
	void OnKeyboardHidden();
	void OnKeyboardWillHide();
	void OnKeyboardBeginChangeFrame(const FRect& begin_frame, const FRect& end_frame, float duration);
	void OnKeyboardEndChangeFrame();
	
private:
	mutable MUTEX_TYPE _mutex;
	
	std::vector<IKeyboardNotificationReceiver*> _receivers;
	
	bool _keyboardActive = false;
	bool _keyboardVisible = false;
};


#endif //__KeyboardManager_H_