#include "stdafx.h"
#include "Utils.h"
#include "CollisionGrid.h"

void CollisionGrid::Create(int fieldWidth, int fieldHeight, int maxObjectWidth, int maxObjectHeight)
{
	_cellWidth = maxObjectWidth + 2;
	_cellHeight = maxObjectHeight + 2;
	CreateGrids(fieldWidth, fieldHeight);
}

void CollisionGrid::StartChecking(float dt)
{
	if (!_objects->empty()) {
		Clear();
		AssignObjectsToGrid();
		Check(dt);
	}
}


void CollisionGrid::CreateGrids(int fieldWidth, int fieldHeight)
{
	int cols = math::ceil(fieldWidth / (float)_cellWidth);
	int rows = math::ceil(fieldHeight / (float)_cellHeight);

	for (int i = 0; i < cols; i++) {
		_grid.push_back(std::vector<std::vector<std::shared_ptr<MovedObject>>>());
		_emptyGrid.push_back(std::vector<int>());
		for (int j = 0; j < rows; j++) {
			_grid[i].push_back(std::vector<std::shared_ptr<MovedObject>>());
			_emptyGrid[i].push_back(0);
		}
	}
}

void CollisionGrid::Clear()
{	
	for (size_t i = 0; i < _grid.size(); i++) 
	{		
		for (size_t j = 0; j < _grid[i].size(); j++)
		{
			_grid[i][j].clear();
			_grid[i][j].shrink_to_fit();
			_emptyGrid[i][j] = 0;
		}
	}
}

void CollisionGrid::SetObjects(std::vector<std::shared_ptr<MovedObject>>* value)
{
	_objects = value;
}

//привязка шаров, находящихся в области ячейки, к каждой ячейке грида
void CollisionGrid::AssignObjectsToGrid()
{	
	for (size_t i = 0; i < _objects->size(); i++) {
		std::shared_ptr<MovedObject> object = _objects->at(i);
		FPoint fp = object->GetCenter();
		int xpos = object->GetCenter().x / _cellWidth;
		int ypos = object->GetCenter().y / _cellHeight;
		if (xpos < 0
			|| ypos < 0
			|| xpos > _grid.size() - 1
			|| ypos > _grid[xpos].size() - 1) 
		{
			continue;
		}
		_grid[xpos][ypos].push_back(object);
		_emptyGrid[xpos][ypos] += 1;
	}	
}

//проверка на столкновение шаров в текущей ячейки и со всеми прилежащими
void CollisionGrid::Check(float dt)
{
	for (size_t i = 0; i < _grid.size(); i++) {
		for (size_t j = 0; j < _grid[i].size(); j++) {
			if (CheckCurrentCell(i, j, dt) == EMPTY) continue;
			CheckNeighboringCell(i, j, i + 1, j, dt);     // проверка с объектами ячейки справа
			CheckNeighboringCell(i, j, i - 1, j + 1, dt); // проверка с объектами ячейки снизу слева
			CheckNeighboringCell(i, j, i, j + 1, dt);     // проверка с объектами ячейки снизу
			CheckNeighboringCell(i, j, i + 1, j + 1, dt); // проверка с объектами ячейки снизу справа
		}
	}
}

int CollisionGrid::CheckCurrentCell(int x, int y, float dt)
{
	if (_emptyGrid[x][y] == EMPTY) return EMPTY;
	if (_emptyGrid[x][y] == 1) return CIRCLES;

	std::vector<std::shared_ptr<MovedObject>>& cell = _grid[x][y];

	for (size_t i = 0; i < cell.size() - 1; i++) {
		std::shared_ptr<MovedObject> object1 = cell[i];
		for (size_t j = i + 1; j < cell.size(); j++) {
			std::shared_ptr<MovedObject> object2 = cell[j];
			CheckCollision(object1, object2, dt);
		}
	}
	return CIRCLES;
}

void CollisionGrid::CheckNeighboringCell(int x1, int y1, int x2, int y2, float dt)
{
	if (x2 < 0 || x2 >= _grid.size() || y2 >= _grid[x2].size()) return;

	if (_emptyGrid[x1][y1] == EMPTY || _emptyGrid[x2][y2] == EMPTY) return;

	std::vector<std::shared_ptr<MovedObject>>& cell0 = _grid[x1][y1];
	std::vector<std::shared_ptr<MovedObject>>& cell1 = _grid[x2][y2];

	for (size_t i = 0; i < cell0.size(); i++) {
		std::shared_ptr<MovedObject> object1 = cell0[i];
		for (size_t j = 0; j < cell1.size(); j++) {
			std::shared_ptr<MovedObject> object2 = cell1[j];
			CheckCollision(object1, object2, dt);
		}
	}
}

void CollisionGrid::CheckCollision(std::shared_ptr<MovedObject> object1, std::shared_ptr<MovedObject> object2, float dt)
{	
	if (object1->IsCanCollisionWithObject(object2) && object1->IsCollision(object2, dt)) {
		object1->CollisionBehaviour(object2);
		object2->CollisionBehaviour(object1);		
	}
}
