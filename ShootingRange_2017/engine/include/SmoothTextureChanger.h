#ifndef __SMOOTHTEXTURECHANGER_H__
#define __SMOOTHTEXTURECHANGER_H__

#pragma once

#include "Render.h"
#include "Utils/random.hpp"
#include "EngineAssert.h"
#include "Render/Text.h"

/// Класс, отвечающий за плавную смену текстур.
/// При смене текстуры уровень альфа-канала старых текстур
/// постепенно уменьшается, новой текстуры - увеличивается.
class SmoothTextureChanger
{
public:
	/// Создание объекта с одной текстурой
	explicit SmoothTextureChanger(Render::Texture *startTexture);

	void setAlphaFactor(float a);

	/// Заменить текстуру сразу, без плавного перехода
	void SetCurrentTexture(Render::Texture *texture);

	Render::Texture* getCurrentTexture();

	/// Установить текст текущей текстуре
	void SetCurrentText(const std::string& text, const std::string& font,
		const IPoint& pos, float scale = 1.0f, const Color& color = Color::WHITE);

	/// Установить текст текущей текстуре
	void SetCurrentText(Render::Text* text, const IPoint& pos);

	/// Установить новую скорость смены текстур
	void SetSpeed(float value);

	/// Добавление новой текстуры в очередь
	void AppendTexture(Render::Texture *newTexture);

	/// Добавление новой текстуры в очередь
	void AppendTextureWithText(Render::Texture *newTexture, const std::string& text, const std::string& font,
		const IPoint& pos, float scale = 1.0f, const Color& color = Color::WHITE);

	/// Добавление новой текстуры в очередь
	void AppendTextureWithText(Render::Texture *newTexture, Render::Text* text, const IPoint& pos);

	/// Есть ли еще текстуры в очереди
	bool HasQueuedImages() const;

	/// Вывод на экран
	void Draw(const IPoint& pos = IPoint(0, 0));

	/// Прошло dt секунд
	void Update(float dt);

private:
	struct Image
	{
		Image();

		Render::Texture* tex = nullptr;
		Render::Text *richText = nullptr;
		IPoint textPos;
		std::string text;
		std::string font;
		float textScale = 1.f;
		Color textColor;
		bool withText;
	};

	float _alphaFactor;

	/// скорость изменения; 1.0f - изменение за 1 секунду
	float _speed = 1.f;

	/// текущая (возможно, уходящая) текстура
	Image _currentImage;

	/// доля текущей текстуры (0; 1]
	float _currentImageShare = 0.f;

	typedef std::deque<Image> Images;

	/// очередь следующих текстур
	/// первая в очереди плавно заменяет _currentImage,
	/// остальные ожидают своей очереди
	Images _nextImages;
};

#endif // __SMOOTHTEXTURECHANGER_H__
