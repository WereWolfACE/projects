#ifndef IMAGE_H_INCLUDED
#define IMAGE_H_INCLUDED

#include "PixelType.h"
#include "platform.h"

#include <boost/shared_array.hpp>

namespace Render {

class GfxFileInfo
{
public:
	GfxFileInfo();

	void InsertImageType(uint32_t imageType);
	void RemoveImageType(uint32_t imageType);
	bool HasImageType(uint32_t imageType) const;
	
public:
	uint8_t imageTypes;
	uint16_t width;
	uint16_t height;
	bool alphaChannel;
};
	
/// Изображение, загруженная в память текстура.
class Image
{
public:
	Image();
	explicit Image(const std::string& filename);

	void GetPropertiesFromFile(const std::string& filename);
	void LoadFromFile(const std::string& filename);

	void ApplyPixelType(EnginePixelType new_type, bool useDithering);
	void FlipVertically();
	
	void Purge();

	static void LoadGfxFileInfo();
	static bool IsGfxFileInfoLoaded();
	static GfxFileInfo* FindGfxFileInfo(const std::string& name, int imageType);
	static void AddGfxFileInfo(const std::string& path, const GfxFileInfo& info);
	
public:
	uint16_t width, height;
	uint32_t compressedSize;

	typedef boost::shared_array<uint8_t> ByteArray;
	
	ByteArray data;

	uint8_t mipLevels;
	uint8_t components;

	EnginePixelType pixelType; ///< Формат пиксельных данных.
	bool alphaChannel;

private:
	typedef std::map<std::string, GfxFileInfo> GfxFileMap;

	static GfxFileMap _gfxFileInfo;
	static bool _gfxFileInfoLoaded;
	
	friend class ImageLoader;
};

} // namespace Render

#endif
