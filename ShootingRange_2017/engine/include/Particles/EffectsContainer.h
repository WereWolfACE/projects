#ifndef __EFFECTSCONTAINER_H__
#define __EFFECTSCONTAINER_H__

#pragma once

#include "Particles/ParticleEffect.h"
#include "Render/SpriteBatch.h"

/// Контейнер эффектов.
/// Позволяет централизованно обновлять и рисовать группу эффектов частиц.
class EffectsContainer
{
public:
	ParticleEffectPtr AddEffect(const std::string& effectName);
	ParticleEffectPtr AddEffect(const std::string& effectName, const FPoint& pos);
	
	/// возвращает true если указатель числится среди живых
	bool IsEffectAlive(const ParticleEffect* eff) const;

	/// возвращает true если эффект с таким именем существует и загружен
	bool IsEffectLoaded(const std::string& effectName) const;

	void Draw(Render::SpriteBatch* batch = nullptr);
	void DrawBlend(Render::SpriteBatch* batch = nullptr);

	void Update(float dt);

	void Pause();
	void Continue();
	void Finish();
	void KillAllEffects();
    
    bool IsFinished() const;

	void SetAlphaFactor(float alphaFactor);
	void SetRGBFactor(float redFactor, float greenFactor, float blueFactor);
	void SetScaleFactor(float scaleX, float scaleY);

	void SetSpriteBatch(Render::SpriteBatchPtr batch);

	void EnumerateEffects(std::function<void(ParticleEffect&)> callback) const;

private:
	typedef std::list<ParticleEffectPtr> ParticleEffects;
	
	ParticleEffects _effects;

	Render::SpriteBatchPtr _batch;
};

#endif // __EFFECTSCONTAINER_H__
