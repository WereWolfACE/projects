#pragma once

#include <RefCounter.h>
#include <boost/intrusive_ptr.hpp>

class IDictionary;

namespace freetype
{
	class Text;
	class FontInstance;
	struct TextStyle;
}

typedef freetype::FontInstance* (createFontFn)(const freetype::TextStyle* textStyle);

class ITextElement : public RefCounter
{
public:
	virtual void print(freetype::Text* target, IDictionary* _dict, const freetype::TextStyle* _textStyle, createFontFn _creatFontFn) = 0;
	virtual std::string toString(IDictionary* _dict) = 0;
};

typedef boost::intrusive_ptr<ITextElement> ITextElementPtr;
