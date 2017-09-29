#pragma once

#include "Utils/utils.hpp"

#if defined(__clang__) || defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnon-virtual-dtor"
#endif

/**
* Объект, позволяющий узнать, является ли пиксель в точке текстуры прозрачным
*/
struct IFlashTextureHitTestDelegate
{
	virtual bool hitTest(float u, float v) = 0;
};

#if defined(__clang__) || defined(__GNUC__)
#pragma GCC diagnostic pop
#endif


namespace Render { class Texture; }
struct FlashTextureInfo
{
public:
	explicit FlashTextureInfo(Render::Texture* l_texture);
	~FlashTextureInfo();
	Render::Texture* GetTextureData();
	void Unload();
	bool IsLoaded() const { return _loaded; }
private:
	Render::Texture* _textureData;
	bool _loaded;
};
