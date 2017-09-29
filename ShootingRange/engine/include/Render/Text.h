#ifndef __TEXT_H__
#define __TEXT_H__

#ifdef _MSC_VER
#pragma once
#endif

#include "Core/Resource.h"
#include "Render/RenderTypes.h"
#include "Utils/FPoint.h"

namespace Render {

typedef boost::intrusive_ptr<class Text> TextPtr;    

class Text : public Resource {
public:
	virtual float getWidth() const = 0;

	virtual void setWidth(float width) = 0;
	
	virtual float getHeight() const = 0;
	
	virtual float getBaseLine() const = 0;

	virtual TextAlign getHAlign() const = 0;

	virtual void setHAlign(TextAlign align) = 0;

	virtual TextAlign getVAlign() const = 0;

	virtual void setVAlign(TextAlign align) = 0;

	virtual float getScale() const = 0;

	virtual void setScale(float scale) = 0;

	virtual const std::string& getFontName() const = 0;

	virtual void setFont(const std::string& font) = 0;

	virtual void setColor(const Color& color) = 0;

	virtual void SetPlain(bool value) = 0;

	virtual void SetSnapToPixels(bool value) = 0;

	virtual void getLettersCoords(std::vector<IPoint>& coords) = 0;

	virtual IPoint GetSize() const = 0;

	virtual void Draw(const FPoint& p = FPoint()) = 0;

	virtual void SetVariable(const std::string& name, const std::string& value) = 0;

	virtual const std::string& GetSource() const = 0;

	virtual void SetSource(const std::string& source) = 0;

	virtual std::string ToString(const std::string& lineSeparatorChar = "\n") = 0;
    
    virtual Render::TextPtr Clone() const = 0;

public:
	static const std::string NOBREAK_SPACE;
	static const std::string NARROW_NOBRAKE_SPACE;
};

} // namespace Render

#endif // __TEXT_H__
