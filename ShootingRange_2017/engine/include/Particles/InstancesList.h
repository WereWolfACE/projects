#pragma once

#include "ThreadSupport.h"

// на MacOS отключаем системный макрос check, иначе будет конфликт с luabind и boost
#if defined(ENGINE_TARGET_MACOS)
#if defined(check)
#undef check
#endif // check
#endif

#include <boost/intrusive/list.hpp>

template<class T>
class InstancesList : public boost::intrusive::list_base_hook<>
{
private:
	class Instances : public boost::intrusive::list<InstancesList, boost::intrusive::constant_time_size<false>>
	{
	public:
		Instances() { instantiated = true; }
		~Instances() { instantiated = false; }

		static bool instantiated;
	};

protected:
	typedef boost::intrusive::list_base_hook<> InstancesListEntry;

#define THREAD_LOCK MUTEX_LOCK_TYPE lock(_mutex)

	InstancesList()
	{
		THREAD_LOCK;
		if (!Instances::instantiated) return;
		_instances.push_back(*this);
	}

	~InstancesList()
	{
		THREAD_LOCK;
		if (!Instances::instantiated) return;
		_instances.erase(Instances::s_iterator_to(*this));
	}

	static void ForAllInstances(std::function<void(InstancesListEntry&)> action)
	{
		THREAD_LOCK;
		if (!Instances::instantiated) return;
		for (auto &i : _instances)
			action(i);
	}

#undef THREAD_LOCK

private:
	static MUTEX_TYPE _mutex;
	static Instances _instances;
};

#define INSTANCES_LIST_DATA \
template<class T> MUTEX_TYPE InstancesList<T>::_mutex;\
template<class T> bool InstancesList<T>::Instances::instantiated = false;\
template<class T> typename InstancesList<T>::Instances InstancesList<T>::_instances;

