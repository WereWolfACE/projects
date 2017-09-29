#ifndef __PARTICLEEFFECT_H__
#define __PARTICLEEFFECT_H__

#pragma once

#include "Core/Resource.h"
#include "Particles/ParticleSystem.h"
#include "Particles/InstancesList.h"
#include "Render/Sheet.h"


/// Эффект из частиц.
/// Представляет собой набор систем частиц @see ParticleSystem
class ParticleEffect : public Resource, public InstancesList<ParticleEffect> {
public:
	typedef std::shared_ptr< std::vector<Render::SheetPtr> > SheetsPtr;
	typedef std::vector<ParticleSystemPtr> ParticleSystems;
	static const uint32_t serializerFourcc = MAKEFOURCC('E', 'f', 'c', 't');
public:
	virtual const std::string& GetName() const override;
	virtual size_t GetMemoryInUse() const override;
	// возвращает размер всех экземпляров кроме ресурсов
	static size_t TotalMemoryInUse();

	ParticleEffect* Clone() const { return new ParticleEffect(*this); }

	// возвращает true если объект является ресурсом, false если копия созданная для воспроизведения эффекта
	inline bool IsResource() const { return GetLoader(); }
	
	template<class Serializer>
	void Serialize(Serializer &serializer, int version);

	void Update(float dt);

	void Draw(Render::SpriteBatch* batch);
	void DrawBlend(Render::SpriteBatch* batch);

	bool HitTest(int x, int y) const;

	const ParticleSystems &GetSystems() const { return _systems; }
	ParticleSystems &GetSystems() { return _systems; }

	bool isEnd() const { return ended; }
	void Kill(void) { ended = true; }
	void Finish();
	void Reset();

	float GetLocalTime() const { return _localTime; }
	void SetLocalTime(float localTime) { _localTime = localTime; }

	void Pause();
	void Continue();
	bool IsPaused() const { return _paused; }
	bool IsPermanent();
	void SetAlphaFactor(float alpha);
	void SetRGBFactor(float r, float g, float b);
	void SetPos(const FPoint& pos);
	void SetPos(float x, float y) { SetPos(FPoint(x, y)); }
	void SetScale(float scale_x, float scale_y = 0.0f);

	// возвращает индексы систем частиц эффекта в правильном порядке
	// если система частиц А ссылается на Б, то Б должна обрабатываться до А
	std::vector<size_t> MakeProcessOrder() const;

	// найти эффект и создать его клон для отображения в игре
	static boost::intrusive_ptr<ParticleEffect> CreateEffect(const std::string& name);
	static boost::intrusive_ptr<ParticleEffect> CreateEffect(const std::string& name, const FPoint& position);

protected:
	virtual void Load(ResourceLoadMode loadMode) override;
	virtual void Unload(ResourceLoadMode loadMode) override;
	virtual void GroupLoad(ResourceLoadManager& glm) override;
	virtual void GroupUnload(ResourceLoadManager& glm) override;

private:
	ParticleEffect();
	ParticleEffect(const ParticleEffect&);
	ParticleEffect& operator=(const ParticleEffect&);

	void CloneSystems(const ParticleEffect& sourceEffect);

	void ProcessResources(std::function<void(Resource *)> processor) const;

	static boost::intrusive_ptr<ParticleEffect> CreateEffectImpl(const std::string& name);
public:
	float posX = 0.0f;
	float posY = 0.0f;
	bool ended = false;

private:
	bool _paused = false;
	float _localTime = 0.0f;
	std::string _name;
	std::string _description;
	
	ParticleSystems _systems;

	friend class ParticleEffectLoader;
	// для редактора эффектов
	friend class EffectDocument;
	friend class SerializationTest;
};

typedef boost::intrusive_ptr<ParticleEffect> ParticleEffectPtr;

#endif // __PARTICLEEFFECT_H__
