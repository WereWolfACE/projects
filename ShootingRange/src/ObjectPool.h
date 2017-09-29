#ifndef __OBJECTPOOL_H__
#define __OBJECTPOOL_H__

#pragma once

template<class T>
struct PoolItem
{
public:
	std::shared_ptr<T> object;
	bool in_use;
};

template<class T>
class ObjectPool
{
public:
	std::shared_ptr<T> Create();
	void Delete(std::weak_ptr<T> object);
	void Delete(T* object);
private:
	std::vector<PoolItem<T>> _objects;
};

template<class T>
std::shared_ptr<T> ObjectPool<T>::Create()
{
	for (size_t i = 0; i < _objects.size(); i++)
	{
		if (!_objects[i].in_use)
		{
			_objects[i].in_use = true;
			return _objects[i].object;
		}
	}

	PoolItem<T> record;
	record.object = std::make_shared<T>();
	record.in_use = true;

	_objects.push_back(record);

	return record.object;
}

template<class T>
void ObjectPool<T>::Delete(std::weak_ptr<T> object)
{
	auto ptr = object.lock();
	if (ptr)
	{
		for (size_t i = 0; i < _objects.size(); i++)
		{
			if (_objects[i].object.get() == ptr.get())
			{
				_objects[i].in_use = false;
				break;
			}
		}
	}
}

template<class T>
void ObjectPool<T>::Delete(T* object)
{	
	for (size_t i = 0; i < _objects.size(); i++)
	{
		if (_objects[i].object.get() == object)
		{
			_objects[i].in_use = false;
			break;
		}
	}	
}
#endif //__OBJECTPOOL_H__

