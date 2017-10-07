#pragma once

template<typename Type>
class StrongRefObjC
{
public:
	StrongRefObjC() :
		_reference(nullptr)
	{
	}

	StrongRefObjC(Type *obj) :
		_reference(nullptr)
	{
		Set(obj);
	}

	StrongRefObjC(const StrongRefObjC &ref) :
		_reference(nullptr)
	{
#if defined(__OBJC__)
		// увеличиваем счётчик ссылок
		Set((__bridge Type*)ref._reference);
#else
		_reference = ref._reference;
#endif
	}

	~StrongRefObjC()
	{
		Set(nullptr);
	}

	StrongRefObjC& operator = (const StrongRefObjC &ref)
	{
		if (this != &ref) {
#if defined(__OBJC__)
			// увеличиваем счётчик ссылок
			Set((__bridge Type*)ref._reference);
#else
			_reference = ref._reference;
#endif
		}
		return *this;
	}

	StrongRefObjC& operator = (Type* obj)
	{
		// увеличиваем счётчик ссылок
		Set(obj);
		return *this;
	}

#if defined(__OBJC__)
	operator Type* () const
	{
		Type* reference = (__bridge Type*)_reference;
//		if (reference) Log::Info("reference retain get is " + utils::lexical_cast(CFGetRetainCount((__bridge CFTypeRef)reference)));
		return reference;
	}
#endif

private:
	void Set(Type *obj)
	{
#if defined(__OBJC__)
		// уменьшаем счётчик ссылок
		Type* oldObject = (__bridge_transfer Type*)_reference;
//		if (oldObject) Log::Info("reference retain set0 is " + utils::lexical_cast(CFGetRetainCount((__bridge CFTypeRef)oldObject)));
//		if (obj) Log::Info("reference retain set1 is " + utils::lexical_cast(CFGetRetainCount((__bridge CFTypeRef)obj)));
		_reference = (__bridge_retained void*)obj;
//		if (obj) Log::Info("reference retain set2 is " + utils::lexical_cast(CFGetRetainCount((__bridge CFTypeRef)obj)));
#else
		_reference = obj;
#endif
	}
private:
	void *_reference;
};
