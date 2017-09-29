#ifndef __SINGLETON_H__
#define __SINGLETON_H__

#pragma once

template <class T>
class SingletonHolder
{
	T* single_ptr;

public:

	SingletonHolder()
	{
		single_ptr = T::Instance();
	}

	~SingletonHolder()
	{
		single_ptr->FreeInstance();
	}

	T* operator ->() {return single_ptr;}

	void ReleaseInstance()
	{
		delete single_ptr;
	}
};

template <class T>
class Singleton
{
	static T* _self;
	static int _refcount;

	friend class SingletonHolder<T>;

protected:
	Singleton() {}
	virtual ~Singleton() {_self = NULL;}

public:
	static T* Instance()
	{
		if (!_self) {
			_self = new T;
			//_refcount++;
		}
		return _self;
	}

    void FreeInstance()
	{
		//if (--_refcount == 0)
		//	delete this;
	}
};

template <class T>
T* Singleton<T>::_self = NULL;

template <class T>
int Singleton<T>::_refcount = 0;

#endif // __SINGLETON_H__
