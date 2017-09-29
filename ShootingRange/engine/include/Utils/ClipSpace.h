#pragma once

#include "FPoint.h"

#include <vector>

/// ¬спомогательный класс дл€ выполнени€ отсечени€ области видимости по пр€мым
struct ClipPlane {
	float a, b, c;

	ClipPlane(float x1, float y1, float x2, float y2) {
		a = y1 - y2;
		b = x2 - x1;
		c = x1 * y2 - x2 * y1;

		const float nn = 1.0f / sqrt(a * a + b * b);

		a *= nn;
		b *= nn;
		c *= nn;
	}

	float factor(float x, float y) const {
		return a * x + b * y + c;
	}

	FPoint intersection(const ClipPlane& other) const {
		float detA = a * other.b - b * other.a;
		float detB1 = -c * other.b + b * other.c;
		float detB2 = -a * other.c + c * other.a;

		Assert(detA != 0.0f);

		return FPoint(detB1 / detA, detB2 / detA);
	}
};

class ClipSpace {
public:
	void setViewport(float x, float y, float w, float h) {
		this->x = x;
		this->y = y;
		this->w = w;
		this->h = h;
	}

	void push(ClipPlane plane) {
		planes.push_back(plane);
	}

	void pop() {
		planes.pop_back();
	}

	bool empty() const {
		return planes.empty();
	}

	/// ¬озвращает список точек в пор€дке обхода по часовой стрелке.
	/// —писок точек формирует выпуклый многоугольник, по которому нужно проводить отсечение.
	std::vector<FPoint>& findPoly() const {
		out.clear();

		out.emplace_back(x, y);
		out.emplace_back(x + w, y);
		out.emplace_back(x + w, y + h);
		out.emplace_back(x, y + h);

		for (size_t i = 0; i < planes.size(); i++) {
			const ClipPlane& plane = planes[i];
			bool positiveFactor = false;
			bool negativeFactor = false;
			const float eps = 1e-3f;
			for (size_t j = 0; j < out.size(); j++) {
				FPoint& v = out[j];
				float f = plane.factor(v.x, v.y);
				if ( f > eps ) {
					positiveFactor = true;
				}
				if (f < -eps) {
					negativeFactor = true;
				}
			}
			if (!negativeFactor) {
				continue;
			}
			if (!positiveFactor) {
				out.clear();
				break;
			}

			FPoint prev = out.back();
			float prevFactor = plane.factor(prev.x, prev.y);
			for (size_t j = 0; j < out.size(); j++) {
				FPoint& v = out[j];
				float f = plane.factor(v.x, v.y);
				if (f < -eps) {
					//point discarded
					if (prevFactor > eps) {
						FPoint iv = plane.intersection(ClipPlane(prev.x, prev.y, v.x, v.y));
						temp.push_back(iv);
					}
				} else {
					//point kept
					if (prevFactor < -eps) {
						FPoint iv = plane.intersection(ClipPlane(prev.x, prev.y, v.x, v.y));
						temp.push_back(iv);
					}
					temp.push_back(v);
				}
				prevFactor = f;
				prev = v;
			}
			out = temp;
			temp.clear();
		}
            
        return out;
	}

private:
	float x, y, w, h;
	mutable std::vector<FPoint> out;
	mutable std::vector<FPoint> temp;
	std::vector<ClipPlane> planes;
};
