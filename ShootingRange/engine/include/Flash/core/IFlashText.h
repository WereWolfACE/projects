#pragma once

#include <FreeType/TextStyle.h>
#include <Render/RenderTypes.h>

#include "IFlashDisplayObject.h"

#if defined(__clang__) || defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnon-virtual-dtor"
#endif

struct IFlashText: public IFlashDisplayObject
{
	virtual void setText(const std::string& text) = 0;
	virtual const std::string& getText() const = 0;
	virtual void setWidth(float width) = 0;
	virtual float getWidth() const = 0;
	virtual void setHeight(float height) = 0;
	virtual float getHeight() const = 0;
	virtual void setVAlign(TextAlign align) = 0;
	virtual TextAlign getVAlign() const = 0;
	virtual void setPrescale(float prescale) = 0;
	virtual float getPrescale() const = 0;
	virtual void setSnapToPixels(bool value) = 0;
	virtual bool getSnapToPixels() const = 0;
	virtual void setSeparateEffects(bool value) = 0;
	virtual bool getSeparateEffects() const = 0;
	virtual void setStyle(const freetype::TextStyle& _style) = 0;
	virtual freetype::TextStyle& getStyle() = 0;
	virtual void cacheAsBitmap(bool enable) = 0;
	virtual bool isCachedAsBitmap() const = 0;
	virtual void suggestFontSize(bool enable) = 0;
	virtual bool issuggestFontSize() = 0;
};

#if defined(__clang__) || defined(__GNUC__)
#pragma GCC diagnostic pop
#endif
