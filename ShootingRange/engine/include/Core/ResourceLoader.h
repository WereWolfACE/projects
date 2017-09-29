#pragma once

#include "RefCounter.h"
#include "AsyncWorkingQueue.h"

class Resource;

///
/// Режим загрузки ресурсов
///
enum class ResourceLoadMode {
	Sync, ///< Синхронный
	Async, ///< Асинхронный
	Post ///< Отложенный
};

///
/// Загрузчик ресурсов.
///
/// Реализует функционал синхронной и асинхронной загрузки ресурсов.
///
/// В классе-наследнике необходимо реализовать три метода:
///	\DoLoadData при асинхронной загрузке вызывается из дополнительного потока, при синхронной - из основного
///	\DoLoadObject всегда вызывается из основного потока, из него следует обращаться к графическому API
///	\DoUnload всегда вызывается из основного потока
///
class ResourceLoader : public RefCounter
{
public:
	ResourceLoader();

	/// Возвращает true во время загрузки
	bool IsLoading() const { return _is_loading; }

	/// Загружает ресурс
	void Load(Resource* resource, ResourceLoadMode mode);

	/// Выгружает ресурс
	void Unload(Resource* resource, ResourceLoadMode mode);

	/// Прерывает асинхронную загрузку
	void CancelLoading();

protected:
	/// Вызывается (при асинхронной загрузке из дополнительного потока) при старте загрузки
	virtual void DoLoadData(Resource* resource) = 0;

	/// Вызывается из основного потока при завершении загрузки
	virtual void DoLoadObject(Resource* resource) = 0;

	/// Вызывается из основного потока при выгрузке
	virtual void DoUnload(Resource* resource) = 0;

private:
	/// Выполняется из основного или дополнительного потока при асинхронной загрузке
	void StartLoad(Resource* resource);

	/// Выполняется только из основного потока
	void EndLoad(Resource* resource, bool error, const char* what);
	
	/// Выполняется только из основного потока
	void EndUnload(Resource* resource);

protected:
	AsyncWorkingQueue::ItemID _queueItemId; ///< Идентификатор элемента в асинхронной очереди

	ResourceLoadMode _mode; ///< В каком режиме была начата загрузка

	bool _is_loading; ///< Происходит ли загрузка прямо сейчас (возможно, асинхронная)

private:
	ResourceLoader(const ResourceLoader&);
	ResourceLoader& operator=(const ResourceLoader&);
};

typedef boost::intrusive_ptr<ResourceLoader> ResourceLoaderPtr;
