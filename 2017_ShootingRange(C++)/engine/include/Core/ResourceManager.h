#pragma once

#include "AsyncWorkingQueue.h"
#include "Core/ResourceFactory.h"
#include "Core/Resource.h"
#include "Core/ResourceGroup.h"
#include "Render/Texture.h"
#include "Render/Text.h"
#include "Render/FreeTypeTexts.h"
#include "ThreadSupport.h"

#include <boost/unordered_map.hpp>

namespace Core {

///
/// Менеджер ресурсов
///
class ResourceManager
{
public:
	ResourceManager();

	/// Добавляет новый ресурс
	Resource* Add(std::type_index type, Resource* res);

	/// Удаляет ресурс указанного типа по его имени
	void Remove(std::type_index type, const std::string& name);

	/// Удаляет все ресурсы указанного типа
	void RemoveAll(std::type_index type);

	/// Удаляет все ресурсы
	void RemoveAll();

	/// Проверяет существование указанного типа ресурса
	bool Exists(std::type_index type) const;

	/// Проверяет существование ресурса указанного типа по имени
	bool Exists(std::type_index type, const std::string& name) const;

	/// Ищет все ресурсы с указанным именем
	void CollectResources(const std::string& name, std::vector<Resource*>& result) const;

	/// Ищет все ресурсы с указанным типом
	void CollectResources(std::type_index type, std::vector<Resource*>& result) const;

	/// Ищет все группы, в которые включён указанный ресурс
	void CollectGroups(const Resource* res, std::vector<std::string>& result) const;

	/// Ищет ресурс указанного типа по имени
	/// \return NULL, если не найден
	Resource* Find(std::type_index type, const std::string& name) const;

	/// Возвращает ресурс по указанному типу и имени.
	/// Если ресурс не найден, выбрасывается исключение.
	Resource* Get(std::type_index type, const std::string& name) const;

	/// Возвращает группу ресурсов по имени.
	/// Если такой группы не существовало, тогда будет создана новая.
	ResourceGroup* GetOrCreateGroup(const std::string& name);

	/// Перезагружает ресурсы указанного типа.
	///
	/// В режимах загрузки ResourceLoadMode::Async и ResourceLoadMode::Post ответственность
	/// за вызов AsyncWorkingQueue::ExecuteAllNow() лежит на вызывающей стороне.
	///
	void Reload(std::type_index type, ResourceLoadMode load_mode);

	/// Перезагружает ресурс указанного типа по имени
	void Reload(std::type_index type, const std::string& name, ResourceLoadMode load_mode);

	/// FIXME специфический хак для восстановления текстур
	void Reload(const std::set<Resource*>& resources, ResourceLoadMode load_mode);

	/// Возвращает размер памяти, занимаемый ресурсами указанного типа
	size_t GetMemoryInUse(std::type_index type) const;

	/// Возвращает суммарный размер занимаемой ресурсами памяти
	size_t GetMemoryInUse() const;

	/// Перечисляет типы хранящихся в менеджере ресурсов
	void EnumerateResourceTypes(std::function<void(std::type_index, size_t count)> callback) const;

	/// Перечисляет хранящиеся в менеджере ресурсы по типу
	void EnumerateResources(std::type_index type, std::function<void(const std::string&, const ResourcePtr&)> callback) const;

	template<typename T> T* Add(Resource* res) {
		static_assert(std::is_base_of<Resource, T>::value, "'res' must be derived from template argument type");
		Assert2(dynamic_cast<T*>(res) != NULL, "'res' must be derived from template argument type");
		return static_cast<T*>(Add(std::type_index(typeid(T)), res));
	}

	template<typename Base, typename T> T* Replace(Resource* res) {
		ResourcePtr guard = res;

		Base* base = Find<Base>(res->GetName());
		if (!base) {
			Halt(std::string("Resource '") + res->GetName() + "' of type '" + typeid(*res).name() + "' wasn't found for replacing");
			return NULL;
		}

		T* object = dynamic_cast<T*>(base);
		if (!object) {
			Halt(std::string("Resource '") + res->GetName() + "' of type '" + typeid(*base).name() + "' is inappropriate");
			return NULL;
		}

		T* proto = dynamic_cast<T*>(res);
		if (!proto) {
			Halt(std::string("Resource '") + res->GetName() + "' of type '" + typeid(*res).name() + "' is inappropriate");
			return NULL;
		}

		*object = *proto;
		return object;
	}

	template<typename T> void Remove(const std::string& name)       { Remove(std::type_index(typeid(T)), name); }
	template<typename T> void RemoveAll()                           { RemoveAll(std::type_index(typeid(T))); }
	template<typename T> bool Exists() const                        { return Exists(std::type_index(typeid(T))); }
	template<typename T> bool Exists(const std::string& name) const { return Exists(std::type_index(typeid(T)), name); }
	template<typename T> void CollectResources(std::vector<Resource*>& result) const { CollectResources(std::type_index(typeid(T)), result); }
	template<typename T> T*   Find(const std::string& name) const   { return static_cast<T*>(Find(std::type_index(typeid(T)), name)); }
	template<typename T> T*   Get(const std::string& name) const    { return static_cast<T*>(Get(std::type_index(typeid(T)), name)); }
	template<typename T> void Reload(ResourceLoadMode load_mode)    { Reload(std::type_index(typeid(T)), load_mode); }
	template<typename T> void Reload(const std::string& name,
		ResourceLoadMode load_mode)                                 { Reload(std::type_index(typeid(T)), name, load_mode); }
	template<typename T> size_t GetMemoryInUse() const              { return GetMemoryInUse(std::type_index(typeid(T))); }

private:
	typedef boost::unordered_map<std::string, ResourcePtr> Resources;
	typedef boost::unordered_map<std::type_index, Resources> ResourceTypes;

	mutable MUTEX_TYPE _mutex;

	ResourceTypes _resources; ///< Коллекция ресурсов
};

extern ResourceManager resourceManager;

} // namespace Core
