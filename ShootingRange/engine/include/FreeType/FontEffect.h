#pragma once

#include "Flash/fquery/fquery.h"
#include "Flash/fquery/fquery_ui.h"

#include "../picojson/picojson.h"

namespace freetype {    
	
class FontEffect
{
public:
	virtual ~FontEffect() {}
	virtual void getExtension(int& nx, int& ny, int& px, int& py, float _prescale) = 0;
	virtual void apply(uint8_t* data, uint8_t* target, int width, int height, int* aabb, float _prescale) = 0;
};
    
class FontEffectTemplate
{
public:
	virtual ~FontEffectTemplate() {}
	virtual freetype::FontEffect* create(picojson::object &object) = 0;
};
    
} // namespace freetype
