#ifndef PIXEL_TYPE_H_INCLUDED
#define PIXEL_TYPE_H_INCLUDED

// Тип хранения цвета пикселя
enum class EnginePixelType : uint8_t
{
	Default,         ///< 
	RGBA8888,        ///< RGBA по 8 бит на каждый канал. В XML: pixelType="8888"	
	RGBA4444,        ///< По 4 бита на каждый канал (16-битный цвет). В XML: pixelType="4444"	
	RGBA5551,        ///< По 5 бит на R, G, B и 1 бит на A. В XML: pixelType="5551"	
	RGB888,          ///< RGB по 8 бит на канал (без альфа-канала). В XML: pixelType="888"	
	RGB565,          ///< 5 бит на R, 6 бит на G, 5 бит на B. В XML: pixelType="565"	
	Gray,            ///< Монохромное изображение 8 бит на пиксел. В XML: pixelType="gray"	
	Alpha,           ///< Только 8-битный альфа канал. В XML: pixelType="alpha"	
	GrayAlpha,       ///< 8 бит интенсивность и 8 бит альфа. В XML: pixelType="gray-alpha"	
	DXT1,            ///< Для сжатых изображений DXT1
	DXT3,            ///< Для сжатых изображений DXT3
	DXT5,            ///< Для сжатых изображений DXT3
	RGB_PVRTC_2BPP,  ///< Для сжатых изображений PVRTC
	RGB_PVRTC_4BPP,  ///< Для сжатых изображений PVRTC
	RGBA_PVRTC_2BPP, ///< Для сжатых изображений PVRTC
	RGBA_PVRTC_4BPP, ///< Для сжатых изображений PVRTC
	ETC1             ///< Для сжатых изображений ETC1
};


EnginePixelType StringToPixelType(const char* str);
std::string PixelTypeToString(EnginePixelType pt);
bool IsCompressedPixelType(EnginePixelType pt);

#endif
