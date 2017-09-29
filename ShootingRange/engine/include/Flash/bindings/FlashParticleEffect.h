#pragma once

#include <Particles/EffectsContainer.h>

#include "../core/FlashDisplayObject.hpp"
#include "../core/FlashUnknown.hpp"

#include "IFlashParticleEffect.h"

class FlashParticleEffect: public FlashDisplayObject<IFlashParticleEffect>
{
BEGIN_INTERFACE_MAP()
	INTERFACE_ENTRY(IFlashDisplayObject)
	INTERFACE_ENTRY(IFlashParticleEffect)
END_INTERFACE_MAP()

public:
	FlashParticleEffect();
	virtual ~FlashParticleEffect();

	void setEffectName(const std::string& _name);
	const std::string& getEffectName() const;

	//------------------------------------------------------------------------------
	//
	//                            IFlashDisplayObject
	//
	//------------------------------------------------------------------------------
	void render(FlashRender& render) override;
	bool hitTest(float x, float y, IHitTestDelegate* hitTestDelegate) override;
	bool getBounds(float &,float &,float &,float &,IFlashDisplayObject *) override;
	bool hasUpdate() const override {return true;};
	void update(float dt) override;

	//------------------------------------------------------------------------------
	//
	//                             IFlashParticleEffect
	//
	//------------------------------------------------------------------------------
	virtual bool isEnd() override;
	virtual void reset() override;
	virtual void finish() override;
	virtual void pause() override;
	virtual void resume() override;

private:
	EffectsContainer container;
	ParticleEffectPtr effect;
	std::string effectName;
	bool wasDrawn;
};
