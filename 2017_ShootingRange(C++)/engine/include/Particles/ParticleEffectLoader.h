#pragma once

#include "Core/ResourceLoader.h"
#include "ThreadSupport.h"
#include "Particles/ParticleEffect.h"

enum class FileStoreMode
{
	Binary,
	Xml
};

// загрузчик эффектов
class ParticleEffectLoader : public ResourceLoader
{
public:
	static bool IsVersionSupported(int version);

	// загрузка файлов эффектов, создаЄт все эффекты файла
	// если group не пуста - эффекты сразу добавл€ютс€ в группу
	// если group равна "Common" - эффекты сразу и навсегда загружаютс€ вместе со всеми текстурами (им вызываетс€ BeginUse)
	// preloadData = true сразу загружает их данные
	static bool LoadEffects(const std::string& filename, const std::string& group, bool preloadData);

	// загрузка файлов эффектов по данным файла ресурсов
	static std::vector<ParticleEffectPtr> LoadEffectsFromResourcesXml(rapidxml::xml_node<> &node, bool preloadData);
protected:
	virtual void DoLoadData(Resource* resource) override;
	virtual void DoLoadObject(Resource* resource) override;
	virtual void DoUnload(Resource* resource) override;

private:
	ParticleEffectLoader() {}
	ParticleEffectLoader(const ParticleEffectLoader&) = delete;
	ParticleEffectLoader& operator = (const ParticleEffectLoader&);

	typedef std::vector<ParticleEffectPtr> Effects;

	static bool LoadEffectsImpl(const std::string& filename, const std::string& group, bool preloadData, Effects &effects);

	static bool Load(IO::InputStream *stream, FileStoreMode mode, const std::string& fileName, EmitterMaskFileReferences &masks, bool preloadData, Effects &loadedEffects, std::stringstream &errors);

	static bool LoadFromXml(IO::InputStream *stream, bool preloadData, Effects &loadedEffects, std::stringstream &errors);
	static bool LoadFromBinary(IO::InputStream *stream, const IO::SerializedContent &content, int version, bool preloadData, Effects &loadedEffects);

	// список масок в pbi-файле
	static EmitterMaskFileReferences GetEmitterMasksParts(IO::InputStream *stream, const IO::SerializedContent &content, int version, const std::string& fileName);

	// загрузить оглавление файла
	static bool LoadBinaryContent(IO::InputStream *stream, IO::SerializedContent &content, int &version, std::stringstream &errors);

	static void LoadEffectTextures(
		bool localTextures,
		ParticleEffect &effect,
		ParticleEffect::SheetsPtr sheets,
		const EmitterMaskFileReferences &masks,
		std::stringstream &warnings,
		std::stringstream &errors);

	static ParticleEffect::SheetsPtr LoadAtlases(const std::string& effectFileName, const std::string& group);

	void PostLoadEffect(ParticleEffect &effect) const;
	static void CopyEffectData(ParticleEffect &dst, const ParticleEffect &src);
	static void ClearEffectData(ParticleEffect &effect);

	void CreateResourcesList(const ParticleEffect &effect);
	void ProcessResources(std::function<void(Resource *)> processor) const;

	inline static ParticleEffect *Cast(Resource* resource);
	inline static ParticleEffectLoader *LoaderCast(ParticleEffect &effect);
	static std::unique_ptr<ParticleEffect> LoadFromXml(IO::InputStream* stream, int version, const std::string &name);
	static std::unique_ptr<ParticleEffect> LoadFromBinary(IO::InputStream* stream, int version);

private:
	std::unique_ptr<ParticleEffect> _effectData;
	bool _preloaded = false;
	int _version = 0;
	std::string _fileName;
	size_t _offset = -1;
	FileStoreMode _storeMode = FileStoreMode::Xml;
	ParticleEffect::SheetsPtr _sheets;
	std::shared_ptr<EmitterMaskFileReferences> _masks;
	std::vector<ResourcePtr> _dependentResources;
	MUTEX_TYPE _loadDataMutex;

	friend class ParticleEffect;
	// дл€ редактора эффектов
	friend class EffectDocument;
};

typedef boost::intrusive_ptr<ParticleEffectLoader> ParticleEffectLoaderPtr;
