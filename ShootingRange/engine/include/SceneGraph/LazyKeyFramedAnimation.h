#ifndef __LAZYKEYFRAMEDANIMATION_H__
#define __LAZYKEYFRAMEDANIMATION_H__

#include "SceneGraph/KeyFramedAnimation.h"

#include "BinaryContentProvider.h"

namespace SceneGraph {

class LazyKeyFramedAnimationPool;

class LazyKeyFramedAnimation: public KeyFramedAnimation {
public:
	typedef boost::intrusive_ptr<LazyKeyFramedAnimation> HardPtr;

	LazyKeyFramedAnimation(BinaryContentProviderPtr& _provider, size_t pos, const std::string& name, float duration, float ticksPerSec);

	virtual void Play();
	virtual void Stop();
	virtual Animation::HardPtr Clone();
	virtual void SetModelNode(ModelNode* modelNode);

protected:
	LazyKeyFramedAnimation(const LazyKeyFramedAnimation& rhs);
	LazyKeyFramedAnimation& operator = (const LazyKeyFramedAnimation& rhs);

private:
	size_t _pos;
	BinaryContentProviderPtr __m_provider; /// провайдер бинарных данных
};
}

#endif // __LAZYKEYFRAMEDANIMATION_H__
