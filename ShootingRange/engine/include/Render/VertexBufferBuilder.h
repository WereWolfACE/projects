#ifndef __VERTEXBUFFERBUILDER_H__
#define __VERTEXBUFFERBUILDER_H__

#pragma once

#include "Render/RenderTypes.h"
#include "Utils/Color.h"

class IRect;
class FRect;

namespace Render {

class VertexBufferBuilder
{
public:
	std::vector<QuadVert> _buffer;

	int numVertices;

public:
	VertexBufferBuilder();
	VertexBufferBuilder(const VertexBufferBuilder& vb);

	virtual ~VertexBufferBuilder() {}

	virtual void Init(int numOfVertices);
	virtual void InitQuadBuffer(int numOfQuads);

	virtual void SetQuad(int quadIndex, float centerX, float centerY, float width, float height, float angle, Color color, float uStart, float uEnd, float vStart, float vEnd);
	
	void SetQuad(int quadIndex, float x, float y, float width, float height, Color color);
	void SetQuad(int quadIndex, float x, float y, float width, float height, const FRect& uv, Color color);
	void SetQuad(int quadIndex, float x, float y, float width, float height, float angle, Color color);
	void SetQuad(int quadIndex, const FRect& rect, const FRect& uv, Color color = Color::WHITE);
	void SetQuad(int quadIndex, const IRect& rect, const FRect& uv, Color color = Color::WHITE);
	
	virtual void SetQuad(int quadIndex, const math::Vector3& v0, const math::Vector3& v1, const math::Vector3& v2, const math::Vector3& v3,
		const Color& c1, const Color& c2, const Color& c3, const Color& c4, float uStart = 0.0f, float uEnd = 1.0f, float vStart = 0.0f, float vEnd = 1.0f);
	
	void SetQuad(int quadIndex, const math::Vector3& v0, const math::Vector3& v1, const math::Vector3& v2, const math::Vector3& v3,
		const Color& color, float uStart = 0.0f, float uEnd = 1.0f, float vStart = 0.0f, float vEnd = 1.0f);
	
	void SetRawData(QuadVert *buf);
	void SetRawData(int vertIndex, QuadVert *buf, int vertCount);

	virtual void Draw();
	
	void DrawAlphaBound();
	
private:
	VertexBufferBuilder& operator=(const VertexBufferBuilder&);
};

class VertexBufferIndexedBuilder : public VertexBufferBuilder
{
public:
	std::vector<uint16_t> _ibuffer;
	int numIndices;
	bool _quadBuffer; // Является ли буффер вместилищем квадов
	int _quadsCount; // Количество квадов при инициализации

public:
	VertexBufferIndexedBuilder();
	VertexBufferIndexedBuilder(const VertexBufferIndexedBuilder &v);

	virtual void Init(int numOfVertices);
	virtual void Init(int numOfVertices, int numOfIndices);
	virtual void Reinit(int numOfVertices, int numOfIndices = 0); 
	virtual void InitQuadBuffer(int numOfQuads);

	/// x, y - центр вращения
	virtual void SetQuad(int quadIndex, float x, float y, float width, float height, float sx, float sy, float angle, Color color,
		float uStart, float uEnd, float vStart, float vEnd);
	
	/// x, y - центр вращения
	void SetQuad(int quadIndex, float x, float y, float width, float height, float angle, Color color);
	void SetQuad(int quadIndex, float x, float y, float width, float height, float angle, Color color,
		float uStart, float uEnd, float vStart, float vEnd);
	
	void SetQuad(int quadIndex, float x, float y, float width, float height, Color color);

	virtual void SetQuad(int quadIndex, const math::Vector3& v0, const math::Vector3& v1, const math::Vector3& v2, const math::Vector3& v3,
		const Color& c1, const Color& c2, const Color& c3, const Color& c4, float uStart = 0.0f, float uEnd = 1.0f, float vStart = 0.0f, float vEnd = 1.0f);
	
	void SetQuad(int quadIndex, const math::Vector3& v0, const math::Vector3& v1, const math::Vector3& v2, const math::Vector3& v3,
		const Color& color, float uStart = 0.0f, float uEnd = 1.0f, float vStart = 0.0f, float vEnd = 1.0f);
	
	virtual void Draw();
	
	void setVertex(int qIndex, const QuadVert& quad);
	void setIndex(int qIndex, int index);
	void setBuffer(const std::vector<QuadVert> &buffer);
	void setIndexBuffer(const std::vector<uint16_t> &ibuffer);

private:
	VertexBufferIndexedBuilder& operator=(const VertexBufferIndexedBuilder&);
};

} // namespace Render

#endif // __VERTEXBUFFERBUILDER_H__
