#ifndef __EMITTERMASK_H__
#define __EMITTERMASK_H__

#pragma once

#include "Core/Resource.h"
#include "Core/ResourceLoader.h"
#include "Utils/random.hpp"
#include "IO/SerializedContent.h"

// часть бинарного файла, откуда нужно грузить данные маски
struct EmitterMaskFileReference
{
	std::string _fileName;
	int _version = 0;
	size_t _offset = 0;

	bool IsValid() const { return !_fileName.empty(); }

	bool operator == (const EmitterMaskFileReference &value) const
	{
		return (_fileName == value._fileName)
			&& (_offset == value._offset)
			&& (_version == value._version);
	}
};

typedef std::map<std::string, EmitterMaskFileReference> EmitterMaskFileReferences;

/// Маска эмиттера
class EmitterMask : public Resource {
public:
	static const uint32_t serializerFourcc = MAKEFOURCC('E', 'M', 's', 'k');
public:
	explicit EmitterMask(const std::string& filename);

	// задать параметры для загрузки маски из pbi-файла
	void SetPbiSource(const std::string& filename, size_t offset, int version = 2);

	// получить параметры для загрузки маски из pbi-файла
	const EmitterMaskFileReference &GetPbiSource() const { return _fileReference; }

	const std::string& GetName() const { return _filename; }

	size_t GetMemoryInUse() const;

	FPoint RandomPoint(float scaleX, float scaleY) const;

	const std::string& GetFileName() const { return _filename; }

private:
	std::string _filename;

	// данные для загрузки маски из pbi-файла
	EmitterMaskFileReference _fileReference;

	bool _usePoints;
	std::vector<IPoint> _points;

	// Ширина и высота изображения
	uint32_t _width, _height;

	std::vector<uint32_t> _distrFunction;
	uint32_t _normFactor;

private:
	friend class EmitterMaskLoader;
	friend class ParticleSystem;
	// для редактора эффектов
	friend class EffectDocument;

	void GetLuminanceData(std::vector<uint8_t>& luminance);
	void CalcDistrFunction(const std::vector<uint8_t>& luminance);

	void LoadMask();
	void UnloadMask();

	template<class Serializer>
	IO::SerializedContent::Entry Serialize(Serializer &serializer, int version, std::vector<uint8_t>& luminance);

private:
	EmitterMask(const EmitterMask&) = delete;
	EmitterMask& operator=(const EmitterMask&) = delete;
};

typedef boost::intrusive_ptr<EmitterMask> EmitterMaskPtr;

/// Загрузчик маски эмиттера
class EmitterMaskLoader : public ResourceLoader {
protected:
	void DoLoadData(Resource* resource);

	void DoLoadObject(Resource* resource);

	void DoUnload(Resource* resource);
};

/// Параметры маски эмиттера
class EmitterMaskParameters : public RefCounter {
public:
	std::string fileName;
	uint8_t luminanceMin = 0;
	uint8_t luminanceMax = 255;
	EmitterMaskParameters() {}
	EmitterMaskParameters(const EmitterMaskParameters &parameters)
		: fileName(parameters.fileName)
		, luminanceMin(parameters.luminanceMin)
		, luminanceMax(parameters.luminanceMax)
	{
	}

private:
	EmitterMaskParameters &operator = (const EmitterMaskParameters &parameters) = delete;
};

typedef boost::intrusive_ptr<EmitterMaskParameters> EmitterMaskParametersPtr;

#endif // __EMITTERMASK_H__
