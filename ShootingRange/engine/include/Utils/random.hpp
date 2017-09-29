#ifndef _UTILS_RANDOM_HPP_
#define _UTILS_RANDOM_HPP_

#include "Utils/FPoint.h"
#include "Utils/Color.h"
#include "Utils/Vector3.h"
#include "Utils/Math.hpp"

namespace math {

/// random seed
void random_seed(size_t value);

/// [min; max]
int random(int min, int max);

/// [min; max]
size_t random(size_t min, size_t max);

/// [min; max)
float random(float min, float max);

/// Возвращает true или false
inline bool random_bool() {
	return random(0, 1) != 0;
}

/// Возвращает 1 или -1
inline int random_sign() {
	return random_bool() ? 1 : -1;
}

/// [0; max]
inline int random(int max) {
	return random(0, max);
}

/// [0; max]
inline size_t random(size_t max) {
	return random(0u, max);
}

/// [0; max)
inline float random(float max) {
	return random(0.0f, max);
}

/// Возвращает случайно отмасштабированный вектор
inline Vector3 random(const math::Vector3& max) {
	return lerp(Vector3::Zero, max, random(0.0f, 1.0f));
}

/// Возвращает вектор случайного размера из диапазона [min; max)
inline Vector3 random(const Vector3& min, const Vector3& max) {
	return lerp(min, max, random(0.0f, 1.0f));
}

/// Все компоненты рандомны [min; max)
inline Color random(const Color& min, const Color& max) {
	return lerp(min, max, random(0.0f, 1.0f));
}

/// Возвращает точку на линии [min; max)
inline FPoint random(const FPoint& min, const FPoint& max) {
	return lerp(min, max, random(0.0f, 1.0f));
}

template <typename T>
class RandomValue {
public:
	RandomValue(T limit, float startPeriod, float endPeriod)
		: _startPeriod(startPeriod)
		, _endPeriod(endPeriod)
		, _period(random(_startPeriod, _endPeriod))
		, _local_time()
		, _limit(limit)
		, _v1(random(T(), _limit))
		, _v2(random(T(), _limit))
		, _value(_v1)
	{
	}

	void Update(float dt) {
		if (_local_time >= _period) {
			_local_time -= _period;
			_period = random(_startPeriod, _endPeriod);
			_v1 = _v2;
			_v2 = random(T(), _limit);
		}
		_value = lerp(_v1, _v2, _local_time / _period);
		_local_time += dt;
	}

	T get() const {
		return _value;
	}

private:
	float _startPeriod;
	float _endPeriod;
	float _period;
	float _local_time;
	T _limit;
	T _v1;
	T _v2;
	T _value;
};

} // namespace math

#endif
