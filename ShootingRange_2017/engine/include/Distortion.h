#ifndef __DISTORTION_H__
#define __DISTORTION_H__

#pragma once

#include "Utils/IPoint.h"
#include "Utils/IRect.h"
#include "Utils/FRect.h"
#include "Render/VertexBufferBuilder.h"
#include "RefCounter.h"

// Назначение: описывает поведение "искажающей сетки"

enum class RefPoint {
	Node,
	Center,
	TopLeft
};

// Узел дисторшена, хранит столбец и строку
struct DistortionNode {
	int col, row;
	
	DistortionNode(int c = 0, int r = 0)
		: col(c)
		, row(r)
	{
	}
};

// Прямоугольник узлов дисторшена, хранит левый нижний угол и првый верхний
struct DistortionNodeRect {
	DistortionNode bottomLeft;
	DistortionNode topRight;

	DistortionNodeRect(int blCol, int blRow, int trCol, int trRow)
		: bottomLeft(blCol, blRow)
		, topRight(trCol, trRow)
	{
	}

	DistortionNodeRect(const DistortionNode &bl, const DistortionNode &tr)
		: bottomLeft(bl)
		, topRight(tr)
	{
	}

	// Возвращает true, если прямоугольник содержит заданный узел
	bool contains(const DistortionNode &dn) const {
		if (dn.col >= bottomLeft.col && dn.col <= topRight.col &&
			dn.row >= bottomLeft.row && dn.row <= topRight.row)
		{
			return true;
		}

		return false;
	}
};

class Distortion : public RefCounter {
public:
	Distortion(int cols, int rows);
	Distortion(const Distortion& dist);
	Distortion(int cols, int rows, std::vector<int> &invis);
	
	// Если invisibleNodes == true, то nodes - массив узлов Distortion'a, подлежащих исключению
	// Если invisibleNodes == false, то nodes - массив узлов Distortion'a, которые НЕ будут исключены
	Distortion(int cols, int rows, const std::vector<DistortionNode> &nodes, bool invisibleNodes = true);

	// rects - массив диапазонов узлов, подлежащих исключению, если invisibleNodes == true, и подлежащих влючению, если invisibleNodes == false
	Distortion(int cols, int rows, const std::vector<DistortionNodeRect> &rects, bool invisibleNodes = true);
	
	void Draw();
	void DrawGrid();
	void SetRenderRect(const IRect& rect, float uStart = 0.0f, float uEnd = 1.0f, float vStart = 0.0f, float vEnd = 1.0f);
	void SetRenderRect(const IRect& rect, const FRect &uv);
	const IRect& GetRenderRect() const { return _rect; }
	const FRect& GetTextureRect() const { return _uv; }
	void SetDisplacement(int col, int row, float dx, float dy, RefPoint ref);
	FPoint GetDisplacement(int col, int row, RefPoint ref) const; 
	void SetColor(int col, int row, Color color);
	void SetColor(Color color);
	void Clear(Color color);
	int getColsCount() const {return _nCols;}
	int getRowsCount() const {return _nRows;}
	void SetTextureCoord(int col, int row, float u, float v);
	float getCellWidth() const { return _cellw; }
	float getCellHeight() const { return _cellh; }

private:
	Distortion();

private:
	Render::VertexBufferIndexedBuilder	_drawBuffer;

	// Размер сетки
	int _nRows, _nCols;
	// Размер ячейки
	float _cellw, _cellh;

	IRect _rect;
	FRect _uv;
};

typedef boost::intrusive_ptr<Distortion> DistortionPtr;

#endif // __DISTORTION_H__
