#include "stdafx.h"
#include "Utils.h"


// проверка, лежат ли две точки в одной полуплоскости относительно прямой, заданной двумя точками
bool Utils::IsPointsAtSamePlain(const FPoint &point1, const FPoint &point2, const FPoint &beginLine, const FPoint &endLine)
{
	double f1 = (point1.x - beginLine.x) / (endLine.x - beginLine.x) -
		(point1.y - beginLine.y) / (endLine.y - beginLine.y);
	double f2 = (point2.x - beginLine.x) / (endLine.x - beginLine.x) -
		(point2.y - beginLine.y) / (endLine.y - beginLine.y);
	if (f1 == 0 && f2 == 0)
	{
		return true;
	}
	if ((f1 > 0 && f2 > 0) || (f1 < 0 && f2 < 0))
	{
		return true;
	}
	return false;
}

// длина вектора (модуль вектора)
double Utils::GetVectorLength(const FPoint &vector)
{
	return math::sqrt(math::sqr(vector.x) + math::sqr(vector.y));
}

// скалярное произведение векторов
double Utils::GetScalar(const FPoint &vector1, const FPoint &vector2)
{
	return vector1.x * vector2.x + vector1.y * vector2.y;
}

// для векторного произведения векторов в двумерном пространстве
double Utils::GetPseudoScalar(const FPoint &vector1, const FPoint &vector2)
{
	return vector1.x * vector2.y - vector1.y * vector2.x;
}

// создать вектор из двух точек
FPoint Utils::GetVector(const FPoint &point1, const FPoint &point2)
{
	return FPoint(point2.x - point1.x, point2.y - point1.y);
}

// получить нормаль из двух точек
FPoint Utils::GetNormal(const FPoint &point1, const FPoint &point2)
{
	FPoint normal;
	normal.x = point1.y - point2.y;
	normal.y = point2.x - point1.x;
	return Normalize(normal);
}

//расстояние от точки до линии
double Utils::GetDistanseFromPointToLine(const FPoint &point, const FPoint &beginLine, const FPoint &endLine)
{
	FPoint vectorToPoint = GetVector(point, beginLine);
	FPoint lineVector = GetVector(beginLine, endLine);
	double distance = math::abs((float)(GetPseudoScalar(vectorToPoint, lineVector) / GetVectorLength(lineVector)));

	return distance;
}

// получить расстояние от точки до точки
double Utils::GetDistanseFromPointToPoint(const FPoint &point1, const FPoint &point2)
{
	FPoint vector = GetVector(point1, point2);
	double length = GetVectorLength(vector);

	return length;
}

// получить нормаль из двух точек
FPoint Utils::Normalize(FPoint vector)
{	
	double length = GetVectorLength(vector);
	vector.x /= length;
	vector.y /= length;

	return vector;
}

int Utils::RandRange(int min, int max)
{
	return rand() % (max + 1 - min) + min;
}

int Utils::RandWithNegative(int number)
{
	return rand() % (number * 2 + 1) - number;
}

FPoint Utils::GetReflectionVector(FPoint beginPoint, FPoint endPoint, FPoint currentVector)
{
	FPoint normal = GetNormal(beginPoint, endPoint);
	double dot = GetScalar(currentVector, normal);
	return FPoint(currentVector.x - 2 * dot * normal.x, currentVector.y - 2 * dot * normal.y);
}

FPoint Utils::GetReflectionCircleVector(FPoint beginPoint, FPoint endPoint, FPoint currentVector)
{
	FPoint vector = GetVector(beginPoint, endPoint);
	FPoint normal = Normalize(vector);
	double dot = GetScalar(currentVector, normal);
	return FPoint(currentVector.x - 2 * dot * normal.x, currentVector.y - 2 * dot * normal.y);
}

