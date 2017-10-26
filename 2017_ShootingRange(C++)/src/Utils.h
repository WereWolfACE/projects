#ifndef __UTILS_H__
#define __UTILS_H__

#pragma once
#include "MovedObject.h"

class Utils
{
public:
	static Utils& GetInstance() {
		static Utils instance;
		return instance;
	}	
	bool IsPointsAtSamePlain(const FPoint &point1, const FPoint &point2, const FPoint &beginLine, const FPoint &endLine);
	double GetVectorLength(const FPoint &vector);
	double GetScalar(const FPoint &vector1, const FPoint &vector2);
	double GetPseudoScalar(const FPoint &vector1, const FPoint &vector2);
	FPoint GetVector(const FPoint &point1, const FPoint &point2);
	FPoint GetNormal(const FPoint &point1, const FPoint &point2);
	double GetDistanseFromPointToLine(const FPoint &point, const FPoint &beginLine, const FPoint &endLine);
	double GetDistanseFromPointToPoint(const FPoint &point1, const FPoint &point2);
	FPoint Normalize(FPoint vector);
	int RandRange(int min, int max);
	int RandWithNegative(int number);
	FPoint GetReflectionVector(FPoint beginPoint, FPoint endPoint, FPoint currentVector);
	FPoint GetReflectionCircleVector(FPoint beginPoint, FPoint endPoint, FPoint currentVector);

private:
	Utils() {}                                
	~Utils() {}
	Utils(Utils const&) = delete;
	Utils& operator= (Utils const&) = delete;

};

#endif //__UTILS_H__

