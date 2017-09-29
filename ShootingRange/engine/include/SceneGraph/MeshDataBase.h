#pragma once

#include <vector>
#include <boost/shared_ptr.hpp>

#include <RefCounter.h>
#include <Render/RenderTypes.h>

namespace SceneGraph {

typedef boost::shared_ptr<class BinaryContentProvider> BinaryContentProviderPtr;

///
/// Данные меша.
///
class MeshDataBase : public RefCounter
{
public:
	typedef std::vector<StaticMeshVert> Vertices;
	typedef std::vector<uint16_t> Indices;

	MeshDataBase(BinaryContentProviderPtr& _provider, size_t offset);
	MeshDataBase(Vertices&& _vertices, Indices&& _indeces);

	size_t GetMemoryInUse() const;

	const Vertices& getVertexData() const { return _vertices; }
		/// Возвращает коллекцию вершин

	const Indices& getIndexData() const { return _indices; }
		/// Возвращает коллекцию индексов

	bool getInitialized() const {return _m_initialized;};

	virtual void init();
	virtual void deinit();

private:
	void LoadFromFile();/// Загружает данные из файла _filename
	void Clear(); /// Удаляет данные

protected:
	BinaryContentProviderPtr _m_provider;
		/// провайдер бинарных данных

	size_t _offset;
		/// Смещение начала данных в файле

	Vertices _vertices;
		/// Данные буфера вершин
	
	Indices _indices;
		/// Данные буфера индексов

	bool _m_initialized;
};

} // namespace SceneGraph
