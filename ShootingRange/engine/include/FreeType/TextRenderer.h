#pragma once

#include <boost/intrusive_ptr.hpp>
#include <Render/SpriteBatch.h>

#include "FreeTypeText.h"
#include "FreeType.h"

namespace freetype
{
class FontInstance;
class CFontGlyphAtlasTexture;
class CTextRenderCache;

class TextRendererBase
{
public:
	TextRendererBase(Render::SpriteBatch* _batch, CFontGlyphAtlasTexture*);

	void getLettersCoords(Text* text, std::vector<IPoint>& coords);
	virtual void render(Text* text, Color color, bool _snapToPixels);

protected:
	void __createRenderTarget(CTextRenderCache* _renderCache, TextRenderMetrics& _metrics) const;
	void __releaseRenderTarget(CTextRenderCache* _renderCache) const;

	void __render(Text* text, Color color, bool _snapToPixels);
	void __renderHelper(Text* text, Color color, bool _snapToPixels);
	virtual const std::vector<LineElement>& getElements(Text* text);

private:
	void __bufferString(FontInstance* fontInstance, const glyphindex* glyphs, int length, Color color, float& x, float& y, float letterSpacing, bool _snapToPixels);

protected:
	boost::intrusive_ptr<Render::SpriteBatch> __spriteBatch;
	CFontGlyphAtlasTexture* fontTexture;
};


class TextRenderer: public TextRendererBase
{
public:
	explicit TextRenderer(CFontGlyphAtlasTexture* _glyphTexture);

public:
	void render(Text* text, Color color, bool _snapToPixels);
};

}
