#pragma once

#include <boost/intrusive_ptr.hpp>
#include <Render/SpriteBatch.h>
#include "Utils/Matrix4.h"

#include "FreeTypeText.h"
#include "FreeType.h"

namespace freetype
{
class FontInstance;
class CFontGlyphAtlasTexture;
class CTextRenderCache;

class TextRendererArcBase
{
public:
	TextRendererArcBase(Render::SpriteBatch* _batch, CFontGlyphAtlasTexture*);
	void render(Text* text, Color color, bool _snapToPixels);

	void setRadius(float _radius) { __m_radius = _radius ;};
	const float getRadius() const {return __m_radius;};

	void setAngle(float _angle){__m_angle =  _angle;};
	const float getAngle() const {return __m_angle;};

protected:
	void __render(Text* text, Color color, bool _snapToPixels);
	void __renderHelper(Text* text, Color color, bool _snapToPixels);
	virtual const std::vector<LineElement>& getElements(Text* text);

private:
	void __bufferString(float _width, FontInstance* fontInstance, const glyphindex* glyphs, int length, Color color, float& x, float& y, float letterSpacing, bool _snapToPixels);

protected:
	boost::intrusive_ptr<Render::SpriteBatch> __spriteBatch;
	CFontGlyphAtlasTexture* fontTexture = nullptr;
	math::Matrix4 __m_glyphsMatrix;
	float __m_radius = 0; //радиус дуги по котогой будет выводится текст
	float __m_angle = 0; //угол(в радианах) раскрыва дуги по кторой будет выводится текст
};


class TextRendererArc: public TextRendererArcBase
{
public:
	explicit TextRendererArc(CFontGlyphAtlasTexture* _glyphTexture);

public:
	void render(Text* text, Color color, bool _snapToPixels);
};

}
