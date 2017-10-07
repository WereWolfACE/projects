#pragma once

#include "RefCounter.h"
#include "ResourceLoader.h"

class ResourceLoadManager;

namespace Core {
	class ResourceManager;
}

///
/// Управляемый ресурс
///
class Resource : public RefCounter
{
public:
	Resource();

	/// Возвращает имя ресурса, уникальное для конкретного типа
	virtual const std::string& GetName() const = 0;

	/// Возвращает размер используемой объектом памяти
	virtual size_t GetMemoryInUse() const = 0;

	/// Возвращает загрузчик ресурса
	ResourceLoader* GetLoader() const { return _loader.get(); }

	/// Устанавливает загрузчик ресурса
	void SetLoader(ResourceLoader* loader) { _loader = loader; }

	/// Проверяет, загружается ли ресурс
	bool IsLoading() const;

	/// Проверяет, загружен ли ресурс
	bool IsLoaded() const { return _is_loaded; }

	/// Возвращает счётчик использования ресурса
	int GetUseCount() const { return _use_count; }

	/// Увеличивает счётчик использования
	void BeginUse(ResourceLoadMode load_mode = ResourceLoadMode::Post);

	/// Уменьшает счётчик использования ресурса
	void EndUse(ResourceLoadMode load_mode = ResourceLoadMode::Post);

	/// Дожидается завершения асинхронной операции, если такая выполняется
	void EnsureLoaded();

	/// Перезагружает загруженный ресурс, отменяя текущую асинхронную загрузку
	void Reload(ResourceLoadMode load_mode);

	/// Получить внешние данные
	intptr_t GetExternData() const { return _extern_data; }

	/// Установить внешние данные
	void SetExternData(intptr_t v) { _extern_data=v; }

protected:
	friend class ResourceGroup;
	friend class Core::ResourceManager;
	friend class ResourceLoadManager;

	/// Загружает незагруженный ресурс
	virtual void Load(ResourceLoadMode load_mode);

	/// Выгружает загруженный ресурс
	virtual void Unload(ResourceLoadMode load_mode);

	/// Планирует вложенные ресурсы на загрузку
	virtual void GroupLoad(ResourceLoadManager& ldm) { }

	/// Планирует вложенные ресурсы на выгрузку
	virtual void GroupUnload(ResourceLoadManager& ldm) { }

	/// Загружает или выгружает ресурс в зависимости от значения счётчика использования
	virtual void SetUseCount(int use_count, ResourceLoadMode load_mode);

protected:
	friend class ResourceLoader;

	int _use_count; ///< Счётчик использования ресурса

	bool _is_loaded; ///< Ресурс загружен?

	intptr_t _extern_data; // Внешние данные зависящие от приложения

	ResourceLoaderPtr _loader;
};

typedef boost::intrusive_ptr<Resource> ResourcePtr;
