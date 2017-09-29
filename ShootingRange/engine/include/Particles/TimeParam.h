#ifndef __TIMEPARAM_H__
#define __TIMEPARAM_H__

#pragma once

#include "Utils/Math.hpp"

class GradientSpline;

namespace IO {
	class BinaryReader;
	class BinaryWriter;
}

class TimeParam
{
public:
	struct Variation {
		float lower;
		float upper;

		Variation() : lower(0), upper(0) { }

		bool operator==(const Variation& rhs) const {
			return
				math::IsEqualFloat(lower, rhs.lower, 0.01f) &&
				math::IsEqualFloat(upper, rhs.upper, 0.01f);
		}

		float Lerp(float t) const {
			if (t < 0.01f)
				return lower;
			if (t > 0.99f)
				return upper;

			return lower + (upper - lower) * t;
		}
	};

	struct SplineKey {
		float time;
		Variation value, lgrad, rgrad;
		bool fixed_grad;

		SplineKey() : time(0), fixed_grad(false) { }

		bool operator==(const SplineKey& rhs) const {
			return
				math::IsEqualFloat(time, rhs.time) &&
				fixed_grad == rhs.fixed_grad &&
				value == rhs.value &&
				lgrad == rhs.lgrad &&
				rgrad == rhs.rgrad;
		}

		template<class Serializer>
		void Serialize(Serializer &serializer, int version, const SplineKey *&previousKey);

	private:
		bool IsRepeat(const TimeParam::SplineKey &key) const;
	};

	typedef std::vector<SplineKey> Keys;

public:
	TimeParam();
	
	bool operator==(const TimeParam& rhs) const;

	size_t MemoryInUse() const;

	template<class Serializer>
	void Serialize(Serializer &serializer, int version);

	void SetScale(float scale) { _scale = scale; }

	float Reset(GradientSpline *spline) const;
	float GetRemoval() const { return _removal; }

	const Keys &GetKeys() const { return _keys; }

	bool IsSimilar() const { return _similar; }
private:
	// для редактора эффектов
	friend class EffectDocument;

	Keys _keys;

	bool _similar;
	float _scale;
	float _removal;
};

#endif // __TIMEPARAM_H__
