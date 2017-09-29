#ifndef __PerformanceTest_h__
#define __PerformanceTest_h__

#if defined(_MSC_VER) && (_MSC_VER > 1300)
#pragma once
#endif

namespace Render {
	class Target;
	class Texture;
}

///
/// »змер€ет производительность видеосистемы
///
class PerformanceTest {
public:
	static const int kTargetSize, kTextureSize, kLoopSize, kEstimateCount;
	
	PerformanceTest();
	~PerformanceTest();
	
	float Estimate(int loopSize = kLoopSize, int estimateCount = kEstimateCount) const;

private:
	float Run(int loopSize) const;
	
	PerformanceTest(const PerformanceTest&);
	const PerformanceTest& operator=(const PerformanceTest&);

private:
	Render::Target* _target;
	Render::Texture* _texture;
};

#endif // __PerformanceTest_h__
