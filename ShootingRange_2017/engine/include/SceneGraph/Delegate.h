#ifndef __DELEGATE_H__
#define __DELEGATE_H__

#pragma once

///
/// Класс делегата.
/// Инкапсулирует указатель на свободную функцию или метод-член класса.
///
template<typename TArgs> class Delegate {
public:
	Delegate()
		: _pObject(0)
		, _pStub(0)
	{
	}
	
	virtual ~Delegate() {}
	
	template<typename TObj, void (TObj::*TMethod)(const void* pSender, TArgs& args)>
	static Delegate Create(TObj* pObject) {
		Delegate aDelegate;
		aDelegate._pObject = pObject;
		aDelegate._pStub = &MethodStub<TObj, TMethod>;
		return aDelegate;
	}
	
	template<void (*TFunction)(const void*, TArgs&)>
	static Delegate Create() {
		Delegate aDelegate;
		aDelegate._pObject = 0;
		aDelegate._pStub = &FunctionStub<TFunction>;
		return aDelegate;
	}
	
	virtual void Invoke(const void* pSender, TArgs& args) {
		(*_pStub)(_pObject, pSender, args);
	}
	
	bool operator == (const Delegate& rhs) const {
		return _pObject == rhs._pObject && _pStub == rhs._pStub;
	}

protected:
	typedef void (*StubType)(void* pObject, const void* pSender, TArgs& args);
	
	void* _pObject;
	StubType _pStub;
	
	template<typename TObj, void (TObj::*TMethod)(const void*, TArgs&)>
	static void MethodStub(void* pObject, const void* pSender, TArgs& args) {
		TObj* p = static_cast<TObj*>(pObject);
		(p->*TMethod)(pSender, args);
	}
	
	template<void (*TFunction)(const void*, TArgs&)>
	static void FunctionStub(void* pObject, const void* pSender, TArgs& args) {
		(*TFunction)(pSender, args);
	}
};

///
/// Класс события.
/// Позволяет подписываться и отписываться нескольким получателям уведомлений.
///
template<typename TArgs> class Event {
public:
	typedef Delegate<TArgs> DelegateType;
	
protected:
	typedef std::vector<DelegateType> Delegates;
	Delegates _delegates;

public:
	Event()
	{
	}
	
	void Add(const DelegateType& delegate) {
		if (std::find(_delegates.begin(), _delegates.end(), delegate) == _delegates.end()) {
			_delegates.push_back(delegate);
		}
	}
	
	void Remove(const DelegateType& delegate) {
		_delegates.erase(std::remove(_delegates.begin(), _delegates.end(), delegate), _delegates.end());
	}
	
	void operator += (const DelegateType& delegate) {
		Add(delegate);
	}
	
	void operator -= (const DelegateType& delegate) {
		Remove(delegate);
	}

	void Clear() {
		_delegates.clear();
	}
	
	void Invoke(const void* pSender, TArgs& args) {
		for (typename Delegates::iterator it = _delegates.begin(); it != _delegates.end(); ++it) {
			(*it).Invoke(pSender, args);
		}
	}
};

///
/// Базовый класс для всех аргументов событий.
///
class EventArgs {
public:
	EventArgs() : _handled(false)
	{
	}
	
	bool IsHandled() const {
		return _handled;
	}
	
	void SetHandled(bool handled = true) {
		_handled = handled;
	}

protected:
	bool _handled;
};

#endif // __DELEGATE_H__
