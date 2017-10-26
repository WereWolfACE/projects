#ifndef _REDNER_RENDERFUNC_H_
#define _REDNER_RENDERFUNC_H_

/// Тут собраны платформо независымые функции для разнообразной отрисовки и прочего

#include "Render/RenderDevice.h"
#include "Render/RenderTypes.h"

namespace Render {

///////////////////////////////////////////////////////

	void BindFont(const std::string& font_name);
	bool isFontLoaded(const std::string &font_name);

	bool IsCharSet( const std::string& font_name, int code);

	void PrintString(int x, int y, const std::string& str, float scale = 1.0f, TextAlign align = LeftAlign, TextAlign valign = TopAlign, bool formatNumbers = true);
	void PrintString(float x, float y, const std::string& str, float scale = 1.0f, TextAlign align = LeftAlign, TextAlign valign = TopAlign, bool formatNumbers = true);
	void PrintString(float x, float y, float _width, const std::string& str, float scale = 1.0f, TextAlign align = LeftAlign, TextAlign valign = TopAlign, bool formatNumbers = true);
	void PrintString(const IPoint& pos, const std::string& str, float scale = 1.0f, TextAlign align = LeftAlign, TextAlign valign = TopAlign, bool formatNumbers = true);
	void PrintString(const FPoint& pos, const std::string& str, float scale = 1.0f, TextAlign align = LeftAlign, TextAlign valign = TopAlign, bool formatNumbers = true);
	void PrintStringInternal(float x, float y, const std::string& str, float scale = 1.0f, TextAlign align = LeftAlign, TextAlign valign = TopAlign);

	float getStringWidth(const std::string& str, const std::string& fontName);
	float getStringWidth(const std::string& str);

	float getFontHeight(const std::string& fontName);
	float getFontHeight();

	float getFontDescender(const std::string& fontName);
	float getFontDescender();

///////////////////////////////////////////////////////

	/// Текущая альфа домножается на multiplier,
	/// старая запоминается в стеке,
	/// восстановление с помощью EndAlphaMul
	///
	/// Множитель ожидается из отрезка [0;1]
	void BeginAlphaMul(float multiplier);
	/// Восстановление старой альфы после BeginAlphaMul
	void EndAlphaMul();

	/// Установка текущего цвета, как произведения заданного и предыдущего.
	/// Используется для управления цветом/альфой для вложенных объектов, значения их цвета/альфы не будут превышать заданных здесь.
	/// Обязательно нужно в конце блока использовать ResetColor, который восстановит исходное значение цвета.
	void SetColor(const Color& color);

	/// Восстанавливает предыдущее сохранённое значение цвета. Используется после SetColor.
	void ResetColor();
	/// Используется для контроля за глубиной стека
	int GetColorStackSize();

	// Псевдоним SetColor с более подходящим названием
	inline void BeginColor(const Color& color) {
		SetColor(color);
	}

	// Псевдоним ResetColor с более подходящим названием
	inline void EndColor() {
		ResetColor();
	}

	void DrawRect(int x, int y, int width, int height, float uStart = 0.0f, float uEnd = 1.0f, float vStart = 0.0f, float vEnd = 1.0f);
	void DrawRect(int x, int y, int width, int height, float factorX, float factorY);
	void DrawRect(const IRect& rect);
	void DrawRect(const IRect& rect, float uStart, float uEnd, float vStart, float vEnd);
	void DrawRect(const IRect& rect, float factorX, float factorY);
	void DrawRect(const IRect& rect, const FRect& frect);
	void DrawRect(const IRect& rect, const FRect& frect, RectOrient orient);
	void DrawRect(const IRect& rect, RectOrient orient);
	void DrawRect(const IRect& rect, float uStart, float uEnd, float vStart, float vEnd, RectOrient orient);

	void DrawRotatedQuad(float x, float y, float width, float height, float angle);
	void DrawQuad(const FRect& rect, const FRect& uv);
	void DrawQuad(const FRect& rect, const FRect& uv, RectOrient orient);
	void DrawQuad(float x, float y, float width, float height);
	void DrawQuad(float x, float y, float width, float height, const FRect& uv);
	void DrawQuad(float x, float y, float width, float height, const FRect& uv, const FRect& uv2);
	void DrawQuad(float x, float y, float width, float height, const FRect& uv, const FRect& uv2, const FRect& uv3);
	void DrawQuad(float x, float y, float width, float height,
				  float u1, float v1, float u2, float v2, float u3, float v3, float u4, float v4,
				  float u21, float v21, float u22, float v22, float u23, float v23, float u24, float v24);
	void DrawQuad(float x, float y, float width, float height, RectOrient orient);
	void DrawQuad(float x, float y, float width, float height, float tex_delta);
	void DrawQuad(float x, float y, float width, float height, float factorX, float factorY);
	void DrawQuad(const math::Vector3& v1, const math::Vector3& v2, const math::Vector3& v3, const math::Vector3& v4
		, float uStart=0.f, float uEnd=1.f, float vStart=0.f, float vEnd=1.f);
	void DrawQuad(const math::Vector3& v1, const math::Vector3& v2, const math::Vector3& v3, const math::Vector3& v4
		, const Color& cv1, const Color& cv2, const Color& cv3, const Color& cv4
		, float uStart=0.f, float uEnd=1.f, float vStart=0.f, float vEnd=1.f);

	// рисуем диагональный блик слева направо, снизу вверх
	// для смены направления на "сверху вниз" нужно передать countX и countY с разными знаками
	void DrawDiagonalBlic(Texture *tex, int countX, int countY, float fi, float time, int alpha, Color c = Color(255, 255, 255), bool draw_base = true);
	void DrawDiagonalBlic(Target *target, int countX, int countY, float fi, float time, int alpha, Color c = Color(255, 255, 255), bool draw_base = true);
	// аналог предыдущей функции, без использования текстуры
	// (хорошо подходит для блика на полностью непрозрачных текстурах);
	// time всегда приводится к [0,1];
	// width определяет ширину бликовой полосы (0 -- отсутствует, 1 -- максимальная);
	// brightness >= 0 влияет на яркость полосы
	void DrawDiagonalGlare(const FRect& rect, int gridRows, int gridCols, float time,
		float width = 1.0f, float brightness = 1.0f, int alpha = 255, Color c = Color(255, 255, 255));

	// появление текстуры слева-направо
	// blurring - количество пикселей размытия (для нечеткой границы)
	void DrawFromLeftToRight(Texture *tex, const FPoint& pos, float part, float blurring);

	// появление текстуры справа-налево
	// blurring - количество пикселей размытия (для нечеткой границы)
	void DrawFromRightToLeft(Texture *tex, const FPoint& pos, float part, float blurring);

	// появление текстуры снизу-вверх
	// blurring - количество пикселей размытия (для нечеткой границы)
	void DrawFromBottomToTop(Texture *tex, const FPoint& pos, float part, float blurring);

	// появление текстуры сверху-вниз
	// blurring - количество пикселей размытия (для нечеткой границы)
	void DrawFromTopToBottom(Texture *tex, const FPoint& pos, float part, float blurring);

	/// Рисует отрезок [p1, p2)
	/// Важно: последний пиксел при этом не рисуется
	void DrawLine(const FPoint& p1, const FPoint& p2);
	void DrawLine(const math::Vector3& p1, const math::Vector3& p2);

	// Рисует внутреннюю рамку прямоугольника толщиной 1 пиксель.
	// То есть если поверх нарисовать DrawRect, то рамка закроется
	// При этом MatrixScale не сможет повлиять на толщину.
	void DrawFrame(const IRect& r);

	// Рисует треугольник
	void DrawTriangle(const FPoint& p1, const FPoint& p2, const FPoint& p3);
	void DrawTriangle(const FPoint& p1, const FPoint& p2, const FPoint& p3, const FPoint& uv1, const FPoint& uv2, const FPoint& uv3);

	/// Рисовалка
	void TrueDraw(const math::Vector3 v[4], const Color c[4], const FPoint uv[4]);
	void TrueDraw(const math::Vector3& v1, const math::Vector3& v2, const math::Vector3& v3, const math::Vector3& v4, Color cv1, Color cv2, Color cv3, Color cv4, const FRect& uv = FRect(0.f, 1.f, 0.f, 1.f));
	void TrueDraw(const math::Vector3& v1, const math::Vector3& v2, const math::Vector3& v3, const math::Vector3& v4, Color cv1, Color cv2, Color cv3, Color cv4, const FPoint& uv1, const FPoint& uv2, const FPoint& uv3, const FPoint& uv4);

	// Проверить, входит ли прямоугольник в единичный квадрат (при текущей текстуре с адресацией AddressMode::Clamp)
	bool CheckUV(const FRect& r);

	/// Push/PopMatrix helper
	class PushMatrix {
	public:
		explicit PushMatrix(MatrixMode mode = MatrixMode::ModelView);
		~PushMatrix();
	private:
		MatrixMode _oldmode;
		MatrixMode _newmode;
	};

	/// Set/ResetColor helper
	class PushColor {
	public:
		explicit PushColor(const Color& color);
		~PushColor();
	};

	/// Begin/EndAlphaMul helper
	class PushAlphaMul {
	public:
		explicit PushAlphaMul(float alpha);
		~PushAlphaMul();
	};

	/// SetBlendMode helper
	class PushBlendMode {
	public:
		explicit PushBlendMode(BlendMode blendMode);
		~PushBlendMode();
	private:
		BlendMode _savedBlendMode;
	};

	class PushTexturing {
	public:
		explicit PushTexturing(bool value);
		~PushTexturing();
	private:
		bool _savedValue;
	};

} // namespace Render

#endif //_REDNER_RENDERFUNC_H_
