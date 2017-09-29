#pragma once

#include <utility>

#include "FreeType.h"

namespace freetype
{

//Задает форматирование параграфа
struct Paragraph
{
	bool wordWrap;

	std::vector<uint32_t> sourceDecimalPoint;
	std::string sourceDecimalPointStr;
	bool groupThousands;
	std::vector<uint32_t> thousandsSeparator;
	std::vector<uint32_t> decimalPoint;

	enum tagWordBreak {
		Normal,
		BreakAll
	} wordBreak;

	enum tagAlign {
		LeftAlign,
		RightAlign,
		CenterAlign,
		WidthAlign
	} align;

	float lineInterval; // 0..1

	Paragraph()
		: align(LeftAlign)
		, wordBreak(BreakAll)
		, wordWrap(true)
		, lineInterval(0.0f)
		, groupThousands(true)
		, thousandsSeparator({ ' ' })
		, decimalPoint({ '.' })
		, sourceDecimalPoint({ '.' })
		, sourceDecimalPointStr(".")
	{
	}

	Paragraph(bool wordWrap, tagWordBreak wordBreak, tagAlign align, float lineInterval
			  , const std::vector<uint32_t>& sourceDecimalPoint
			  , const std::string& sourceDecimalPointStr
			  , bool groupThousands
			  , const std::vector<uint32_t>& thousandsSeparator
			  , const std::vector<uint32_t>& decimalPoint
			  )
		: align(align)
		, wordBreak(wordBreak)
		, wordWrap(wordWrap)
		, lineInterval(lineInterval)
		, sourceDecimalPoint(sourceDecimalPoint)
		, sourceDecimalPointStr(sourceDecimalPointStr)
		, groupThousands(groupThousands)
		, thousandsSeparator(thousandsSeparator)
		, decimalPoint(decimalPoint)
	{
	}


};

//-----------------------------------------------------------------------------
//
//           Задает слово - участок текста с постоянным шрифтом.
//
//-----------------------------------------------------------------------------
class Word
{
public:
	Word(FontInstance* font, uint32_t* charcodes, int length, Color color, float letterSpacing = 0);
	virtual ~Word();

	FontInstance* getFont() const;
	Color getColor() const;
	float getLetterSpacing() const;
	virtual const std::vector<glyphindex>& getGlyphs();
	virtual bool update();

protected:
	virtual void setGlyphs(const std::vector<glyphindex>& glyphs);
	FontInstance* fontInstance = nullptr;
	Color color;

	float letterSpacing;


private:
	std::vector<glyphindex> glyphs;
};

//-----------------------------------------------------------------------------
//
// Число в тексте - выделено в отдельную сущьность для авто форматирования
//
//-----------------------------------------------------------------------------
class Number: public Word
{
public:
	Number(FontInstance* font, uint32_t* charcodes, int length, const Color& color, float letterSpacing);
	Number(FontInstance* font, uint32_t* charcodes, int length, const Color& color, bool is_signed, float letterSpacing, int16_t _lGlue, int16_t _rGlue);
	virtual ~Number();

	void setSettings(bool groupThousands, const std::vector<uint32_t>& thousandsSeparator, const std::vector<uint32_t>& defDecimalPoint, const std::vector<uint32_t>& decimalPoint);
	virtual const std::vector<glyphindex>& getGlyphs() override; // non-const! происходит обновление объекта

	int16_t getlGlue() const { return __m_lGlue; }
	int16_t getrGlue() const { return __m_rGlue; }

protected:
	virtual void setGlyphs(const std::vector<glyphindex>& glyphs);

private:
	int16_t __m_lGlue;
	int16_t __m_rGlue;

	void updateNumberGlyphs();
	std::vector<glyphindex> grouped_glyphs;
	bool isSigned;
	bool groupThousands;
	std::vector<uint32_t> defDecimalPoint;
	std::vector<uint32_t> thousandsSeparator;
	std::vector<uint32_t> decimalPoint;

	std::vector<glyphindex> glyphThousandsSeparator;
	std::vector<glyphindex> glyphDecimalPoint;
	std::vector<glyphindex> glyphDefDecimalPoint;
};

//-----------------------------------------------------------------------------
//
// Динамическое слово - обращается к переменной для получения своего значения
//
//-----------------------------------------------------------------------------
template<class BaseClass, class T, typename Format>
class DynamicWord : public BaseClass
{
public:
	DynamicWord(FontInstance* font, Color color, const T* _ref, float letterSpacing = 0):
		BaseClass(font, 0, 0, color, letterSpacing),
		ref(_ref),
		cached(*_ref)
	{
		__update();
	}

	virtual ~DynamicWord()
	{
	}

	bool update()
	{
		if (__ischnaged())
		{
			__update();
			BaseClass::update();
			return true;
		}

		return  false;
	}

private:
	bool __ischnaged()
	{
		if (*ref == cached)
		{
			return false;
		}

		cached = *ref;
		return true;
	}

	void __update()
	{
		format(ref, chars);

		if (!chars.empty())
		{
			std::vector<glyphindex> new_glyphs;
			new_glyphs.reserve(chars.size());

			freetype::resolveCharCodes(BaseClass::fontInstance, &(chars[0]), chars.size(), new_glyphs);
			BaseClass::setGlyphs( new_glyphs );
		};
	}

private:
	std::vector<uint32_t> chars;

	Format format;
	const T* ref;
	T cached;
};

//---------------------------------------------------------------------------
//
//                     Встраиваемый объект (например, картинка)
//
//---------------------------------------------------------------------------
class InlinedObject
{
public:
	virtual ~InlinedObject(){}
	virtual void getSize(float& width, float& height) = 0;
	virtual void draw(float penX, float penY, Color color) = 0;
};

//---------------------------------------------------------------------------
//
//                                TextRenderMetrics
//
//---------------------------------------------------------------------------
struct TextRenderMetrics
{
	TextRenderMetrics()
	{
	}

	TextRenderMetrics(float _top, float  _left, float _width, float _height) :
		top(_top),
		left(_left),
		width(_width),
		height(_height)
	{
	}

	inline float getTop() const
	{
		return top;
	};

	inline float getRigth() const
	{
		return left + width;
	};

	inline float getBottom() const
	{
		return top + height;
	};

	inline float getLeft() const
	{
		return left;
	};

	inline float getWidth() const
	{
		return width;
	};

	inline float getHeight() const
	{
		return height;
	};

private:
	float top = 0;
	float left = 0;

	float width = 0;
	float height = 0;
};

//---------------------------------------------------------------------------
//
//                               LineElement
//
//---------------------------------------------------------------------------
struct LineElement
{
	float width;
	int spaces;
	int16_t leftGlue;
	int16_t rightGlue;

	enum {
		InlinedObject,
		Text,
		LineBreak,
		Paragraph
	} type;

	union {
		struct {
			float ascender;
			float spacing;
			float leading;
		};

		freetype::InlinedObject* object;
		freetype::Paragraph* paragraph;

		struct{
			FontInstance* font;
			const glyphindex* glyphs;
			int length;

			uint32_t color;
			float letterSpacing;
		} text;
	};
};

//---------------------------------------------------------------------------
//
//                                Text
//
//---------------------------------------------------------------------------
class Text
{
public:
	explicit Text(float prescale);
	~Text();

	int addParagraph(Paragraph* paragraph);
	int addWord(Word* word);
	int addNumber(Number* number);
	int addObject(InlinedObject* object);

	float getLeft() const { return left; }
	void setLeft(float left);

	float getTop() const { return top; }
	void setTop(float top);

	float getRight() const { return right; }
	void setRight(float right);

	float getBottom() const { return bottom; }
	void setBottom(float bottom);

	float getWidth() const {return right - left;};
	float getHeight() const {return top - bottom;};

	float getPrescale() const { return _prescale; }
	void setPrescale(float prescale);

	void move(float x, float y);

	Paragraph* getLastParagraph();
	void getMetrics(float _width, TextRenderMetrics* _metrics);
	void getMetrics(TextRenderMetrics* _metrics);
	void getLineMetrics(float _width, float* lineAscender, float* lineSpacing, float* lineLeading);
	int getLinesNumber(float _width);
	const std::vector<LineElement>& getElements();
	bool isUpdated();

	void setRenderCache(RefCounter* _renderCache){ __m_RenderCache = _renderCache; }
	RefCounter* getRenderCache(){ return __m_RenderCache.get(); }

	static Paragraph DefaultParagraph;

private:
	void __updateElements();
	void __getElements(float _width, std::vector<LineElement>& _line);
	void __getMetrics(std::vector<LineElement>& line, float _width, TextRenderMetrics* _metrics);
	LineElement __newLineElm(float& x, float& y, float _lineAscender, float _lineSpacing, float _lineLeading, int& lineSpaces, bool& isLineEmpty) const;
	LineElement __breakLineElm(float& x, float& y, float &_lineAscender, float& lineSpacing, float& _lineLeading, int& lineSpaces, bool& isLineEmpty) const;
	LineElement __paragraphElm(Paragraph* _paragraph) const;
	LineElement __inlineElm(InlinedObject* _inlineObj, float &lineBearing) const;
	LineElement __textElm(Word* _text, size_t _tbegin, size_t _tend, float _lineInterval, float &_lineAscender, float& lineSpacing, float& _lineLeading, int& lineSpaces) const;

	LineElement& __wordWrap(const LineElement& _textElement, std::vector<LineElement>& _line, float _width, float& _x, float& _y, float& _lineAscender, float& _lineSpacing, float _lineLeading, int& _lineSpaces, bool& _isLineEmpty);
	LineElement& __wordBreak(LineElement& _textElement, std::vector<LineElement>& _line, float _width, float& _x, float& _y, float& _lineAscender, float& _lineSpacing, float _lineLeading, int& _lineSpaces, bool& _isLineEmpty);

private:
	float left, top, right, bottom, _prescale;

	struct TextElement{
		union {
			Paragraph* paragraph;
			Word* word;
			Number *number;
			InlinedObject* inlinedObject;
		};

		enum {
			PARAGRAPH,
			NUMBER,
			WORD,
			OBJECT
		} tag;
	};

	std::list<TextElement> elements;
	std::vector<LineElement> __m_line;
	Paragraph* lastParagraph;

	boost::intrusive_ptr<RefCounter> __m_RenderCache;
};

typedef std::vector<LineElement>::const_iterator LineElementConstIter;
typedef std::vector<LineElement>::iterator LineElementIter;

} // namespace freetype
