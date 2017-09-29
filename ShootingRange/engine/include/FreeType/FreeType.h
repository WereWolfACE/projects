#pragma once

#include <vector>
#include <Utils/Color.h>

#define GLYPH_INDEX_MASK       0x0fffffff
#define GLYPH_ATTR_MASK        0xf0000000

#define GLYPH_ATTR_SPACE       0x10000000
#define GLYPH_ATTR_TAB         0x20000000
#define GLYPH_ATTR_NEWLINE     GLYPH_ATTR_SPACE | GLYPH_ATTR_TAB
#define GLYPH_ATTR_UNBREAKABLE 0x40000000

#define GET_GLYPH_INDEX(value) (value & GLYPH_INDEX_MASK)
#define GET_GLYPH_ATTR(value) (value & GLYPH_ATTR_MASK)

namespace freetype {
	struct glyphindex {
		uint32_t value = 0;
		// символ нельзя отрывать от предыдущего (точку, запятую, многоточие от слова и т.п.)
		bool unbreakable = false;
		// символ можно отделять всегда (иероглифы в японском, китайском, корейском)
		bool separate = false;

		glyphindex() : value() {}
		glyphindex(uint32_t index) : value(index) {} // non-explicit!

		operator uint32_t() const { return value; }
	};

	const uint32_t USERGLYPHS = 0xffffff00;
	const uint32_t SPACE = 0xffffff10;
	const uint32_t TAB = 0xffffff20;
	const uint32_t NEWLINE = 0xffffff30;
	const uint32_t SOFT_HYPHEN = 0xffffff40;
	const uint32_t ZEROWIDTHJOINER = 0xffff200D;
	const uint32_t ZEROWIDTHSPACE = 0xffff200B;

	//представление глифа на текстуре
	struct GPUGlyph {
		float u1, v1, u2, v2;
		float width, height;
		float offsetX, offsetY;
		float advanceX, advanceY;
	};

	class CFontGlyphAtlasTexture;
	class FontInstance;
	class FontEffect;
	class FontEffectTemplate;

	void InitFreetype();
	void UninitFreetype();

	CFontGlyphAtlasTexture* createFontTexture(int width, int height);
	void destroyFontTexture(CFontGlyphAtlasTexture*);

	void applyFontEffect(FontInstance* instance, FontEffect* effect);
	void setFontColor(FontInstance* instance, Color color);
	float getFontCharWidth(FontInstance* instance, uint32_t charcode);
	float getFontKerning(FontInstance* instance, uint32_t charcode, uint32_t next);
	float getFontSpaceWidth(FontInstance* instance);
	void setFontSpaceWidth(FontInstance* instance, float multiplier);

	struct DropShadow {
		int blurX;
		int blurY;
		int offset;
		float angle;
		float power;
		Color color;
		bool inner;
	};

	FontEffect* createDropShadowEffect(DropShadow* descriptor);
	
	struct GradientFill {
		Color colors[256];
		float ratios[256];
		int anchors;
	};

	FontEffect* createGradientFillEffect(GradientFill* descriptor);

	struct Stroke {
		int strokeTop = 0;
		int strokeBottom = 0;
		int strokeLeft = 0;
		int strokeRight = 0;
		Color color = Color::BLACK;
	};

	FontEffect* createStrokeEffect(Stroke* descriptor);

	FontEffect* createSkewEffect(float angle);

	void destroyFontEffect(FontEffect* effect);

	void resolveCharCodes(FontInstance* fontInstance, const uint32_t* charcodes, int count, std::vector<glyphindex>& glyphs);

	float calculateStringWidth(FontInstance* instance, const glyphindex* glyphs, int length, float letterSpacing = 0);
	float getStringAscender(FontInstance* instance);
	float getStringDescender(FontInstance* instance);
	float getLineHeight(FontInstance* instance);

	void discardGlyphs(CFontGlyphAtlasTexture* texture);
	GPUGlyph* getGlyph(FontInstance* fontInstance, CFontGlyphAtlasTexture* texture, glyphindex glyph);
	float getKerning(FontInstance* fontInstance, glyphindex glyph1, glyphindex glyph2);
	float getSpaceWidth(FontInstance* fontInstance);
} // namespace freetype
