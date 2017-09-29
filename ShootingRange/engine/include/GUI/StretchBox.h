#ifndef _GUI_STRETCHBOX_H_
#define _GUI_STRETCHBOX_H_

#include "Render/Texture.h"

/**
 * Класс для отрисовки текстуры в прямоугольник произвольного размера,
 * с растягиванием внутренней части без растягивания углов.
 * Используется, к примеру, для отрисовки панелей с однородным в середине
 * фоном (в т.ч. для тултипов).
 * 
 * Есть также режим тайлинга.
 * 
 * Если размеры отрисовываемого прямоугольника отрицательные, то изображение
 * будет отражено.
 */
class StretchBox
{
public:
	enum class StripeType {
		Fixed,
		Stretched
	};
	
	struct Stripe {
		int size;
		StripeType type;
		
		Stripe(int s = 0, StripeType t = StripeType::Fixed) : size(s), type(t) {}
		void InitFromXml(rapidxml::xml_node<>* elem);
	};

protected:
	Render::Texture * tex;

	bool tiled; /// Использовать тайлинг вместо стретча.
	bool no_center; /// Не отрисовывать центр (для рамок)

	std::vector<Stripe> vertStripes, horStripes;

public:
	StretchBox()
		: tex()
		, tiled(false)
		, no_center(false)
	{ }

	StretchBox(Render::Texture *tex, const std::vector<Stripe>& hor, const std::vector<Stripe>& vert, bool tiled = false, bool no_center = false)
		: tex(tex)
		, horStripes(hor)
		, vertStripes(vert)
		, tiled(tiled)
		, no_center(no_center)
	{ }

	StretchBox(Render::Texture * tex, int left, int right, int top, int bottom, bool tiled = false, bool no_center = false)
		: tex(tex)
		, tiled(tiled)
		, no_center(no_center)
	{
		GenHorStripes(left, right);
		GenVertStripes(top, bottom);
	}

    void SetTexture(Render::Texture* t);

	void initFromXml(rapidxml::xml_node<>* xmlElement);

	void Draw(FRect rect) { Draw(rect.xStart, rect.xEnd, rect.yStart, rect.yEnd); }
	void Draw(IRect rect) { Draw(rect.x + 0.f, rect.x + rect.width + 0.f, rect.y + 0.f, rect.y + rect.height + 0.f); }
	void Draw(FPoint p, float w, float h) { Draw(p.x, p.x + w, p.y, p.y + h); }
	void Draw(float x1, float x2, float y1, float y2);

	/// Проверка прозрачности точки.
	/// x, y отсчитываются от левого нижнего угла прямоугольника w x h.
	bool isPixelTransparent(float x, float y, float w, float h) const;

	Render::Texture* getTexture() { return tex; }

	/// Установить стандартное разбиение 3x3 -
	/// середина растягивается (или тайлится), углы - нет.
	void GenHorStripes(int left, int right);
	void GenVertStripes(int top, int bottom);

	/// Возвращает минимальный размер прямоугольника в виде IPoint.
	IPoint getMinSize() const;
	/// Возвращает размеры текстуры в виде прямоугольника IRect(IPoint(0, 0), w, h).
	IRect getDefaultSize() const;

protected:
	// Вспомогательные методы

	void DrawPart(float x1, float y1, float x2, float y2,
		float tx1, float ty1, float tx2, float ty2, float ww, float hh) const;

	void DrawTiled(float x, float y, float w, float h,
		float tx, float ty, float tw, float th, float ww, float hh) const;
};

#endif // _GUI_STRETCHBOX_H_
