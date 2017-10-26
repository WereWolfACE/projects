#pragma once

#include "Resource.h"

#include <boost/unordered_set.hpp>

///
/// Группа ресурсов
///
class ResourceGroup : public Resource
{
public:
	explicit ResourceGroup(const std::string& name);

	~ResourceGroup();

	const std::string& GetName() const { return _name; }

	size_t GetMemoryInUse() const { return sizeof(*this); }

	/// Добавляет ресурс в группу
	void Add(Resource* res);

	/// Удаляет ресурс из группы
	void Remove(Resource* res);

	/// Удаляет все ресурсы из группы
	void RemoveAll();

	/// Проверяет, содержится ли указанный ресурс в группе
	bool Contains(const Resource* res) const;

	/// Проверяет, содержится ли указанный по имени ресурс в группе
	bool Contains(const std::string& name) const;

	/// Заполняет контейнер ресурсами, принадлежащими группе
	template<typename Container>
	void GetResources(Container& resources) const {
		resources.reserve(resources.size() + _resources.size());
		for (Resources::const_iterator it = _resources.begin(); it != _resources.end(); ++it) {
			resources.push_back((*it).get());
		}
	}

protected:
	void GroupLoad(ResourceLoadManager& glm);

	void GroupUnload(ResourceLoadManager& glm);

	void Load(ResourceLoadMode load_mode = ResourceLoadMode::Post);

	void Unload(ResourceLoadMode load_mode = ResourceLoadMode::Post);

	void SetUseCount(int use_count, ResourceLoadMode load_mode);

private:
	std::string _name;

	typedef boost::unordered_set<ResourcePtr> Resources;

	Resources _resources; ///< Коллекция дочерних ресурсов
};

typedef boost::intrusive_ptr<ResourceGroup> ResourceGroupPtr;
