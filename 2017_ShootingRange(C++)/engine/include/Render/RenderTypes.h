#ifndef __RENDERTYPES_H__
#define __RENDERTYPES_H__

#pragma once

#include "Utils/Color.h"
#include "Utils/Vector3.h"
#include "PixelType.h"

enum class RectOrient
{
	Normal,
	Rotate90,
	Rotate180,
	Rotate270
};

enum TextAlign
{
	LeftAlign,
	RightAlign,
	CenterAlign,
	TopAlign,
	BottomAlign,
	BaseLineAlign
};

enum class DeviceMode
{
	Fullscreen,
	Windowed
};

enum class MatrixMode
{
	ModelView,
	Projection,
	
	Count
};

/// @warning От порядка записи имён зависит выбор шейдера в @see RenderDeviceGLES2.cpp
enum class ShaderType {
	Standard,
	StandardUv2,
	StandardUv3,
	AddColor,
	AlphaOnly,
	NoAlpha,
	NoTexture,
	Mesh,
	TextureMatrix,
	ReplaceAlpha,
	ReplaceAlphaNoVertexColor,
	ReplaceAlphaUv2,
	MultiplyAlpha,
	MultiplyAlphaUv2,
    Stencil,
	CubeMap,
	CompensateColor
};

enum class BlendMode
{
	None,              ///< смешивание выключено
	Alpha,
	Multiply,          ///< не применяется в режиме premultiplied alpha
	Add,
	Replace,           ///< не применяется в режиме premultiplied alpha
	InvMultiply,       ///< инверсный multiply
	InvMultiplyMask,
	PremultipliedAlpha
};

/// @warning От порядка записи имён зависит @see RenderDeviceGLES2::SetStencilFunc()
enum class StencilFunc {
	Never,
	Less,
	LessOrEqual,
	Greater,
	GreaterOrEqual,
	Equal,
	NotEqual,
	Always
};

/// @warning От порядка записи имён зависит @see RenderDeviceGLES2::SetStencilOp()
enum class StencilOp {
	Keep,
	Zero ,
	Replace,
	Increment,
	IncrementWrap,
	Decrement,
	DecrementWrap,
	Invert
};

enum class TextureType {
	Texture,
	CubeTexture,
	VolumeTexture,
	Target,

	// TODO: Эти удалить!
	PartialTexture,
	CombinedTexture
};

enum class FilteringType : uint8_t {
	Nearest,
	Bilinear
};

enum class AddressMode : uint8_t {
	Repeat,
	Clamp
};

enum StageOp
{
	STAGE_DEFAULT = 0,     // режим совместимости со старым кодом

	STAGE_A_SKIP = 0x0000,        // не использовать альфу
	STAGE_A_REPLACE = 0x0001,     // заменить альфу
	STAGE_A_MODULATE = 0x0002,    // умножить альфу
	STAGE_A_INVMODULATE = 0x0003, // умножить на 1-A
	STAGE_A_ADD = 0x0004,         // сложить альфу
	STAGE_A_SUB = 0x0005,         // вычесть альфу
	STAGE_A_INTERPOLATE = 0x0006, // линейная интерполяция от предыдущего значения до текущего (задаётся альфой операции)

	STAGE_C_SKIP = 0x0000,        // не использовать цвет
	STAGE_C_REPLACE = 0x0100,     // заменить цвет
	STAGE_C_MODULATE = 0x0200,    // умножить цвет
	STAGE_C_INVMODULATE = 0x0300, // умножить на 1-C
	STAGE_C_ADD = 0x0400,         // сложить цвет
	STAGE_C_SUB = 0x0500,         // вычесть цвет
	STAGE_C_INTERPOLATE = 0x0600, // линейная интерполяция от предыдущего значения до текущего (задаётся альфой операции)
	STAGE_C_BLEND = 0x0700,        // смешение цвета Cp*(1-As)+Cs*As
		
	STAGE_PROJECTED = 0x00010000,

	STAGE_A_MASK = 0x00ff,
	STAGE_C_MASK = 0xff00
};

enum MultisampleType
{
	MULTISAMPLE_NONE = 0,
	MULTISAMPLE_2_SAMPLES,
	MULTISAMPLE_3_SAMPLES,
	MULTISAMPLE_4_SAMPLES,
	MULTISAMPLE_5_SAMPLES,
	MULTISAMPLE_6_SAMPLES,
	MULTISAMPLE_7_SAMPLES,
	MULTISAMPLE_8_SAMPLES,
	MULTISAMPLE_9_SAMPLES,
	MULTISAMPLE_10_SAMPLES,
	MULTISAMPLE_11_SAMPLES,
	MULTISAMPLE_12_SAMPLES,
	MULTISAMPLE_13_SAMPLES,
	MULTISAMPLE_14_SAMPLES,
	MULTISAMPLE_15_SAMPLES,
	MULTISAMPLE_16_SAMPLES
};

struct RenderTargetConfig
{
	RenderTargetConfig()
		: pixelType(EnginePixelType::RGBA8888)
		, depthBuffer(false)
		, stencilBuffer(false)
		, multisampleType(MULTISAMPLE_NONE)
	{ }

	EnginePixelType pixelType;
	bool depthBuffer;
	bool stencilBuffer;
	MultisampleType multisampleType;
};

enum class BufferUsage {
	Static,
	Dynamic
};

enum class PrimitiveType {
	PointList,
	LineList,
	LineStrip,
	TriangleList,
	TriangleStrip,
	TriangleFan
};

enum class VertexElementUsage {
	Position,
	Normal,
	BlendWeights,
	BlendIndices,
	Color,
	TexCoord,
	TexCoord1,
	TexCoord2
};

enum class VertexElementType {
	Byte,
	UByte,
	Short,
	UShort,
	Int,
	UInt,
	Float
};

struct VertexElementDescription {
	VertexElementUsage usage;
	size_t size;
	VertexElementType type;
	bool normalized;
	size_t stride;
	size_t offset;
};

#define BEGIN_VERTEX_DECLARATION(VertexType) \
static const VertexElementDescription* GetDeclaration() { \
	typedef VertexType ThisType; \
	static VertexElementDescription descr[] = {

#define DECLARE_VERTEX_ELEMENT(Usage, Size, Type, Normalized, Field) \
		{ Usage, Size, Type, Normalized, sizeof(ThisType), offsetof(ThisType, Field) },

#define END_VERTEX_DECLARATION() \
		{ VertexElementUsage::Position, 0, VertexElementType::Float, false, 0, 0 } \
	}; \
	return descr; \
}

struct QuadVert {
	float x, y, z;
	Color color;
	float u, v;

	BEGIN_VERTEX_DECLARATION(QuadVert)
		DECLARE_VERTEX_ELEMENT(VertexElementUsage::Position, 3, VertexElementType::Float, false, x)
		DECLARE_VERTEX_ELEMENT(VertexElementUsage::Color, 4, VertexElementType::UByte, true, color)
		DECLARE_VERTEX_ELEMENT(VertexElementUsage::TexCoord, 2, VertexElementType::Float, false, u)
	END_VERTEX_DECLARATION()
};

struct QuadVertT2 {
	float x, y, z;
	Color color;
	float u1, v1;
	float u2, v2;

	BEGIN_VERTEX_DECLARATION(QuadVertT2)
		DECLARE_VERTEX_ELEMENT(VertexElementUsage::Position, 3, VertexElementType::Float, false, x)
		DECLARE_VERTEX_ELEMENT(VertexElementUsage::Color, 4, VertexElementType::UByte, true, color)
		DECLARE_VERTEX_ELEMENT(VertexElementUsage::TexCoord, 2, VertexElementType::Float, false, u1)
		DECLARE_VERTEX_ELEMENT(VertexElementUsage::TexCoord1, 2, VertexElementType::Float, false, u2)
	END_VERTEX_DECLARATION()
};

struct QuadVertT3 {
	float x, y, z;
	Color color;
	float u1, v1;
	float u2, v2;
	float u3, v3;

	BEGIN_VERTEX_DECLARATION(QuadVertT3)
		DECLARE_VERTEX_ELEMENT(VertexElementUsage::Position, 3, VertexElementType::Float, false, x)
		DECLARE_VERTEX_ELEMENT(VertexElementUsage::Color, 4, VertexElementType::UByte, true, color)
		DECLARE_VERTEX_ELEMENT(VertexElementUsage::TexCoord, 2, VertexElementType::Float, false, u1)
		DECLARE_VERTEX_ELEMENT(VertexElementUsage::TexCoord1, 2, VertexElementType::Float, false, u2)
		DECLARE_VERTEX_ELEMENT(VertexElementUsage::TexCoord2, 2, VertexElementType::Float, false, u3)
	END_VERTEX_DECLARATION()
};

struct QuadVertCubeMap {
	float x, y, z;
	Color color;
	float u, v, w;

	BEGIN_VERTEX_DECLARATION(QuadVertCubeMap)
		DECLARE_VERTEX_ELEMENT(VertexElementUsage::Position, 3, VertexElementType::Float, false, x)
		DECLARE_VERTEX_ELEMENT(VertexElementUsage::Color, 4, VertexElementType::UByte, true, color)
		DECLARE_VERTEX_ELEMENT(VertexElementUsage::TexCoord, 3, VertexElementType::Float, false, u)
	END_VERTEX_DECLARATION()
};

struct StaticMeshVert {
	math::Vector3 position;
	math::Vector3 normal;
	float tu, tv;

	BEGIN_VERTEX_DECLARATION(StaticMeshVert)
		DECLARE_VERTEX_ELEMENT(VertexElementUsage::Position, 3, VertexElementType::Float, false, position)
		DECLARE_VERTEX_ELEMENT(VertexElementUsage::Normal, 3, VertexElementType::Float, false, normal)
		DECLARE_VERTEX_ELEMENT(VertexElementUsage::TexCoord, 2, VertexElementType::Float, false, tu)
	END_VERTEX_DECLARATION()
};

struct SkinnedMeshVert {
	math::Vector3 position;
	float weights[4];
	uint8_t indices[4];
	math::Vector3 normal;
	float tu, tv;

	BEGIN_VERTEX_DECLARATION(SkinnedMeshVert)
		DECLARE_VERTEX_ELEMENT(VertexElementUsage::Position, 3, VertexElementType::Float, false, position)
		DECLARE_VERTEX_ELEMENT(VertexElementUsage::BlendWeights, 4, VertexElementType::Float, false, weights)
		DECLARE_VERTEX_ELEMENT(VertexElementUsage::BlendIndices, 4, VertexElementType::UByte, false, indices)
		DECLARE_VERTEX_ELEMENT(VertexElementUsage::Normal, 3, VertexElementType::Float, false, normal)
		DECLARE_VERTEX_ELEMENT(VertexElementUsage::TexCoord, 2, VertexElementType::Float, false, tu)
	END_VERTEX_DECLARATION()
};

struct QuadVertStencil {
	float x, y, z;

	BEGIN_VERTEX_DECLARATION(QuadVertStencil)
		DECLARE_VERTEX_ELEMENT(VertexElementUsage::Position, 3, VertexElementType::Float, false, x)
	END_VERTEX_DECLARATION()
};

#endif // __RENDERTYPES_H__
