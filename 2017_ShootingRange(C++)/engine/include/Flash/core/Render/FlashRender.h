#pragma once

#include <Utils/Vector3.h>
#include <Utils/Color.h>

#include "IDeviceRender.h"
#include "IFlashTextRenderingEngine.h"
#include "types.h"
#include "ClippingMode.h"

//-----------------------------------------------------------------------------
static const FlashConstantId FlashTextureCh0 = 1;
static const FlashConstantId FlashBlendMode = 2;
static const FlashConstantId FlashShaderCh0 = 3;
static const FlashConstantId FlashMVPMatrix = 4;
static const FlashConstantId FlashClipRect = 5;

static const FlashEnum FlashBlendNormal = 0;
static const FlashEnum FlashBlendAdd = 1;
static const FlashEnum FlashBlendMultiply = 2;

static const int FLASH_MAX_CONSTANTS = 64;
static const int VERTEX_BUFFER_SIZE = 8096;

/**
* Используется для рисования FlashDisplayObject'ов
*
* Вместо конкретных функций вроде setBlendMode... использует именованные константы.
* Например, blendMode задается при помощи setConstantEnum(FlashBlendMode, ...)
* Сам по себе FlashRender ничего не знает о этих константах и лишь передает в IDeviceRender,
* который уже должен правильно интерпретировать их.
* 
* Такое решение принято в связи с тем, что при использовании шейдеров невозможно угадать, какие
* именно параметры нужно будет передавать.
*/
class FlashRender
{
public:
	FlashRender();
	~FlashRender();

	/**
	* Сохраняет текущую матрицу в массив localStorage (предполагается выделить его на стеке)
	*/
	void saveMatrix(float localStorage[6]) const;

	/**
	* Загружает матрицу из массива m
	*/
	void restoreMatrix(const float m[6]);

	/**
	* Умножает текущую матрицу на m
	*/
	void multMatrix(const float m[6]);

	/**
	* Преобразует точку (x,y) при помощи текущей матрицы
	*/
	void transform(float& x, float& y) const;
	/**
	* Сохраняет текущую матрицу в localStorage, а затем умножает ее на m
	*/
	void saveMultMatrix(const float m[6], float localStorage[6]);

	/**
	* Сохраняет текущий цвет в localStorage, а затем умножает его на v
	*/
	Color setColor(const Color& _color);
	const Color& getColor() const;

	bool isTransformDirect() const;

	/**
	* Устанавливает "физическое" устройство вывода. Подробнее
	* @see IDeviceRender
	*/
	void setDeviceRender(IDeviceRender* device);
	IDeviceRender* getDeviceRender() const;

	bool needSaveConstant(FlashConstantId id) const;

	FlashConstant saveConstant(FlashConstantId id) const;

	void restoreConstant(FlashConstantId id, FlashConstant constant);

	void unsetConstant(FlashConstantId id);

	void invalidateConstant(FlashConstantId id);

	FlashShader setConstantShader(FlashConstantId id, FlashShader x);

	FlashShader getConstantShader(FlashConstantId id) const;

	FlashEnum setConstantEnum(FlashConstantId id, FlashEnum x);

	FlashEnum getConstantEnum(FlashConstantId id) const;

	float setConstantFloat(FlashConstantId id, float x);

	float getConstantFloat(FlashConstantId id) const;

	void setConstantVector(FlashConstantId id, float* v);

	void getConstantVector(FlashConstantId id, float* v) const;

	void setConstantMatrix3x2(FlashConstantId id, float* v);

	void getConstantMatrix3x2(FlashConstantId id, float* v) const;

	void setConstantMatrix4x4(FlashConstantId id, float* v);

	void getConstantMatrix4x4(FlashConstantId id, float* v) const;

	void* setTexture(FlashConstantId id, void* _textureData);
	void* getTexture(FlashConstantId id) const;

	/**
	* Функцию следует вызывать, если предполагается низкоуровневая работа с устройством в обход FlashRender
	*/
	void invalidateConstants();

	/**
	* Очищает внутренний буфер и выводит все накопленные четырехугольники на устройство.
	*/
	void Draw(const math::Vector3& vec1, const math::Vector3& vec2, const math::Vector3& vec3, const math::Vector3& vec4, Color color, const FRect& uv);
	void flush();
	void Begin();
	void End();

	void renderText(TextObjectOutputParams& params);

	void beginClipPlane(float x1, float y1, float x2, float y2);
	void endClipPlane();

	void beginScissorTest(const IRect& rect, ClippingMode clippingMode);
	void endScissorTest();

public:
	float matrix[6];
	Color color;

private:
	void __validateConstants();

private:
	bool constantsValid;

	FlashConstant constants[FLASH_MAX_CONSTANTS];

	IDeviceRender* device;
};
