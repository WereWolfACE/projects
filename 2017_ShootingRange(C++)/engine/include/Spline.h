#ifndef __SPLINE_H__
#define __SPLINE_H__

#pragma once

#include "EngineAssert.h"
#include "Utils/FPoint.h"
#include "Utils/Xml.h"
#include "Utils/Math.hpp"

float SplineInterpolation(float x1, float x2, float r1, float r2, float t);
float GetGradient(float x1, float x2, float r1, float r2, float t);

FPoint SplineInterpolation(const FPoint& v1, const FPoint& v2, const FPoint& g1, const FPoint& g2, float t);
FPoint GetGradient(const FPoint& v1, const FPoint& v2, const FPoint& g1, const FPoint& g2, float t);

// Этот класс едва ли где-нибудь используется.
template <class T>
class SplinePath {
public:
	typedef std::pair<T, T> Key;
	typedef std::vector<Key> Keys;

	SplinePath() = default;

	explicit SplinePath(std::initializer_list<T> list) {
		_keys.reserve(list.size());

		for (auto&& v: list) {
			_keys.emplace_back(v, T());
		}

		CalculateGradient();
	}

	void Clear() {
		Keys().swap(_keys);
	}

	void addKey(const T& key) {
		_keys.emplace_back(key, T());
	}

	int GetKeysCount() const {
		return static_cast<int>(_keys.size());
	}

	const Key& GetKey(int i) const {
		Assert(i >= 0 && i < GetKeysCount());
		return _keys[i];
	}

	Key& GetKey(int i) {
		Assert(i >= 0 && i < GetKeysCount());
		return _keys[i];
	}

	T getFrame(int sector, float t) {
		Assert(t >= 0.0f && t <= 1.0f);
		size_t i = static_cast<size_t>(sector);
		return SplineInterpolation(_keys[i].first, _keys[i+1].first, _keys[i].second, _keys[i+1].second, t);
	}

	T getGradient(int sector, float t) {
		Assert(t >= 0.0f && t <= 1.0f);
		size_t i = static_cast<size_t>(sector);
		return GetGradient(_keys[i].first, _keys[i+1].first, _keys[i].second, _keys[i+1].second, t);
	}

	T getGlobalFrame(float t) {
		Assert(t >= 0.0f && t <= 1.0f);
		int sectors = getSectors();
//		Assert(sectors > 0);
		if (sectors <= 0) {
			return T();
		}
		float timeStep = (1.0f/sectors);
		int tessSector = static_cast<int>(t/timeStep);
		if (tessSector >= sectors) {
			return _keys.back().first;
		}
		float tessLocalTime = (t-tessSector*timeStep)/timeStep;
		return getFrame(tessSector, tessLocalTime);
	}

	T getGlobalGradient(float t) {
		Assert(t >= 0.0f && t <= 1.0f);
		int sectors = getSectors();
		Assert(sectors > 0);
		if (sectors <= 0) {
			return T();
		}
		float timeStep = (1.0f/sectors);
		int tessSector = static_cast<int>(t/timeStep);
		if (tessSector >= sectors) {
			return _keys.back().second;
		}
		float tessLocalTime = (t-tessSector*timeStep)/timeStep;
		return getGradient(tessSector, tessLocalTime);
	}

	int getSectors() const {
		Assert(_keys.size() > 0);
		return static_cast<int>(_keys.size()-1);
	}

	void CalculateGradient(bool cycled = false) {
		Assert(!_keys.empty());

		if (_keys.empty())
			return;

		if (cycled) {
			_keys.push_back(_keys[0]);
		}

		if (cycled) {
			_keys[0].second = 0.5f*(_keys[1].first - _keys[_keys.size() - 2].first);
			_keys[_keys.size() - 1].second = _keys[0].second;
		} else {
			if (_keys.size() > 1) {
				_keys[0].second = _keys[1].first - _keys[0].first;
				_keys[_keys.size()-1].second = _keys[_keys.size()-1].first - _keys[_keys.size()-2].first;
			}
		}

		for (size_t i = 1; i < _keys.size()-1; i++) {
			_keys[i].second = 0.5f * (_keys[i+1].first - _keys[i-1].first);
		}
	}

protected:
	Keys _keys;
};

typedef SplinePath<float> FSpline;

template <typename T = float>
class TimedSpline {
public:
	TimedSpline() = default;

	explicit TimedSpline(std::initializer_list<std::pair<float, T>> list)
	{
		_keys.reserve(list.size());

		for (auto&& p: list) {
			_keys.emplace_back(p.second, p.first);
		}

		CalculateGradient();
	}

	void Clear() {
		Keys().swap(_keys);
		_currentSector = 1;
	}

	void addKey(float time, T key) {
		Assert(time >= 0.0f);
		_keys.emplace_back(key, time);
	}

	int GetKeysCount() const {
		return static_cast<int>(_keys.size());
	}

	const T& GetKey(int i) const {
		Assert(i >= 0 && i < GetKeysCount());
		return _keys[i].value;
	}

	void SetKey(int i, const T& key) {
		Assert(i >= 0 && i < GetKeysCount());
		_keys[i].value = key;
	}

	void CalculateGradient() {
		std::sort(_keys.begin(), _keys.end(), [](const KeyFrame &a, const KeyFrame &b) {
			return a.time < b.time;
		});

		size_t e = _keys.size() - 1;

		_keys[0].ts = 0.5f * (_keys[1].value - _keys[0].value) * (_keys[1].time - _keys[0].time);
		_keys[e].td = 0.5f * (_keys[e].value - _keys[e - 1].value) * (_keys[e].time - _keys[e - 1].time);

		for (size_t i = 1; i < e; i++) {
			_keys[i].ts = _keys[i].td = 0.5f * (_keys[i + 1].value - _keys[i - 1].value);

			float inv_dd = 1.0f / (_keys[i + 1].time - _keys[i - 1].time);

			float k1 = 2.0f * (_keys[i].time - _keys[i - 1].time) * inv_dd;
			float k2 = 2.0f * (_keys[i + 1].time - _keys[i].time) * inv_dd;

			_keys[i].td *= k1;
			_keys[i].ts *= k2;
		}
	}

	T getGlobalFrame(float t) const {
		Assert(_keys.empty() ? true : t >= _keys.front().time && t <= _keys.back().time);

		if (t < 0.0001f) {
			_currentSector = 1;
			return _keys.front().value;
		}
		else if (t > _keys.back().time - 0.0001f) {
			_currentSector = 1;
			return _keys.back().value;
		}

		size_t sector = getSector(t);
		float localTime = math::clamp(0.f, 1.0f, t - _keys[sector].time) / (_keys[sector + 1].time - _keys[sector].time);

		return getFrame(sector, localTime);
	}

	T getGlobalGradient(float t) const {
		Assert(_keys.empty() ? true : t >= _keys.front().time && t <= _keys.back().time);
		size_t sector = getSector(t);
		float localTime = (t - _keys[sector].time) / (_keys[sector + 1].time - _keys[sector].time);
		return getGradient(sector, localTime);
	}

	bool empty() const {
		return _keys.empty();
	}

private:
	size_t getSector(float t) const {
		const float EPS = 0.0001f;

		auto size = _keys.size();

		// Должно быть минимум два ключа, иначе ничего работать не будет.
		Assert(size > 1);

		// Проверка на то, что мы в начале времени (для ускорения поиска)
		if (t < EPS) /* t <= 0 */
			_currentSector = 1;

		// На случай, если время пойдёт вспять
		while (_currentSector > 0 && _keys[_currentSector - 1].time >= t)
			_currentSector--;

		// Линейное восхождение к нужному значению
		while (_currentSector < size && _keys[_currentSector].time < t)
			_currentSector++;

		// Если ничего не найдено
		// Условия выше гарантируют невозможность аварийной ситуации,
		// то есть если что-то пошло не так, первый ключ будет считаться искомым.
		if (_currentSector == 0 || _currentSector == size)
			_currentSector = 1;

		// В этой точке currentKey указывает на тот ключ,
		// у которого time БОЛЬШЕ ЛИБО РАВЕН t, либо на первый ключ,
		// если ничего не найдено.

		return _currentSector - 1; // Это граница, по которой вычисляеются параметры расчёта точки сплайна.
	}

	T getFrame(size_t sector, float t) const {
		Assert(t >= 0.0f && t <= 1.0f);
		auto&& k1 = _keys[sector];
		auto&& k2 = _keys[sector + 1];
		return SplineInterpolation(k1.value, k2.value, k1.ts, k2.td, t);
	}

	T getGradient(size_t sector, float t) const {
		Assert(t >= 0.0f && t <= 1.0f);
		auto&& k1 = _keys[sector];
		auto&& k2 = _keys[sector + 1];
		return GetGradient(k1.value, k2.value, k1.ts, k2.td, t);
	}

private:
	struct KeyFrame {
		T value;
		T td, ts;
		float time;

		KeyFrame(T key, float time)
			: value(key)
			, time(time)
			, td()
			, ts()
		{
		}
	};

	typedef std::vector<KeyFrame> Keys;

	Keys _keys;

	mutable size_t _currentSector = 1;
};

// Этот класс используется во flash fx
class TimedSplinePath : public TimedSpline<float> {
public:
	TimedSplinePath() = default;

	explicit TimedSplinePath(std::initializer_list<std::pair<float, float>> list)
		: TimedSpline<float>(list)
	{
	}

	/*
	<SomeSplineName>
		<key time="" value="" />
		...
	</SomeSplineName>
	*/
	TimedSplinePath(rapidxml::xml_node<>* node, bool calculateGradient = true)
	{
		for (rapidxml::xml_node<>* key = node->first_node("key"); key; key = key->next_sibling("key")) {
			addKey(Xml::GetFloatAttribute(key, "time"), Xml::GetFloatAttribute(key, "value"));
		}

		if (calculateGradient) {
			CalculateGradient();
		}
	}
};

// Этот класс используется в эффектах.
class GradientSpline {
public:
	struct Key {
		float value;
		float left_gradient;
		float right_gradient;
		float time;
		bool fixed_gradient;

		Key() = default;

		Key(float t, float v, float lgrad, float rgrad, bool fixed)
			: time(t)
			, value(v)
			, left_gradient(lgrad)
			, right_gradient(rgrad)
			, fixed_gradient(fixed)
		{
		}
	};

	GradientSpline() = default;

	size_t MemoryInUse() const;

	void Clear();

	void sizeHint(size_t size) { _keys.reserve(size); }

	void addKey(float time, float value, float lgrad = 0, float rgrad = 0, bool fixed_grad = false);

	void deleteKey(size_t pos) { _keys.erase(std::next(_keys.begin(), pos)); }

	size_t size() const { return _keys.size(); }

	const Key& operator[](size_t pos) const { return _keys[pos]; }

	void CalculateGradient();

	float getGlobalFrame(float time) const;
	float getGlobalGradient(float time) const;

private:
	static float interpolationValue(float time, const Key &key1, const Key &key2);
	static float interpolationGradient(float time, const Key &key1, const Key &key2);

private:
	typedef std::vector<Key> Keys;
	Keys _keys;

	// Номер того элемента массива _keys, который сейчас обрабатывается
	// в getGlobalFrame  в соответствии со временем.
	// Начинается от 1, так как нулевой элемент _keys
	// никогда не может быть искомым в поиске.
	mutable uint16_t currentKeyFrame = 1;
};


// Этот класс разрабатывался для редактора эффектов, но сейчас не используется.
class TrackSpline {
public:
	TrackSpline() = default;

	struct Key {
		float time;
		FPoint value, gradient_in, gradient_out;
		bool linear_in;
		bool linear_out;
	};

	typedef std::vector<Key> Keys;

	void Clear() {
		Keys().swap(_keys);
	}

	void addKey(float x, float y, bool linear_in = false, bool linear_out = false) {
		_keys.emplace_back();
		Key &key = _keys.back();
		key.value.x = x;
		key.value.y = y;
		key.linear_in = linear_in;
		key.linear_out = linear_out;
	}

	void deleteKey(size_t pos) {
		_keys.erase(std::next(_keys.begin(), pos));
	}

	size_t size() const {
		return _keys.size();
	}

	Key& operator[](size_t pos) {
		return _keys[pos];
	}

	void Calculate();

	FPoint getGlobalFrame(float time);

	FPoint getGlobalGradient(float time);

	FPoint interpolationValue(float time, size_t i, size_t j) const;

	FPoint interpolationGradient(float time, size_t i, size_t j) const;

	size_t Prev(size_t i) const;

	size_t Next(size_t i) const;

	float Length(const Key &key1 , const Key &key2) const;

	FPoint GradientIn(size_t i) const;

	FPoint GradientOut(size_t i) const;

private:
	Keys _keys;

	bool _cycled = false;

	float _length = 0.0f;
};

#endif // __SPLINE_H__
