#ifndef __COLLISIONGRID_H__
#define __COLLISIONGRID_H__

#pragma once

#include "MovedObject.h"

class CollisionGrid
{
public:
	void Create(int width, int height, int maxWidth, int maxHeight);
	void StartChecking(float dt);
	void SetObjects(std::vector<std::shared_ptr<MovedObject>>* value);

private:
	static const int EMPTY = 0;
	static const int CIRCLES = 1;

	void AssignObjectsToGrid();
	void Check(float dt);
	int CheckCurrentCell(int x, int y, float dt);
	void CheckNeighboringCell(int x1, int y1, int x2, int y2, float dt);
	void CheckCollision(std::shared_ptr<MovedObject> object1, std::shared_ptr<MovedObject> object2, float dt);
	void Clear();
	void CreateGrids(int fieldWidth, int fieldHeight);

private:	
	int _cellWidth;
	int _cellHeight;
	std::vector<std::shared_ptr<MovedObject>>* _objects;
	std::vector<std::vector<std::vector<std::shared_ptr<MovedObject>>>> _grid;
	std::vector<std::vector<int>> _emptyGrid;
};

#endif //__COLLISIONGRID_H__

