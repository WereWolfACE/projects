#ifndef __PARTICLE_H__
#define __PARTICLE_H__

#include "Spline.h"
#include "ParticleSystem.h"

//  ласс частицы с динамически измен€ющимис€ свойствами.
class Particle {
public:
	Particle();
	Particle(const Particle& rhs);

	void SetOwner(const ParticleSystem::Parameters* owner) { _owner = owner; }

	void Update(float dt);
	void Reset();

	float GetOrientationAngle() const;

	size_t MemoryInUse() const;

public:
	float localTime;

	float lifeTime;
	float spinStart;

	float emitX;
	float emitY;

	float xStart;
	float yStart;

	float x, y, size, angle, v, spin, ySize;
	float red, green, blue, alpha, fps;

	float _speed;
	float _progress;
	uint16_t _currentFrame = 0, _firstFrame = 0, _lastFrame = 0;

	bool isVisible;
	bool isPaused = false;

private:
	Particle& operator=(const Particle&) = delete;

	void UpdateFrames(float dt, float normTime);
	void ResetFrames();

private:
	const ParticleSystem::Parameters* _owner;
	GradientSpline xSpl, ySpl, sizeSpl, angleSpl, vSpl, spinSpl, redSpl, greenSpl, blueSpl, alphaSpl, fpsSpl, ySizeSpl;
};

#endif // __PARTICLE_H__
