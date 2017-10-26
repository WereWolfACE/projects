#pragma once
#include "Game.h"

class ShootingRangeWidget : public GUI::Widget
{
public:
	ShootingRangeWidget(const std::string& name, rapidxml::xml_node<>* elem);

	void Draw() override;
	void Update(float dt) override;
	
	void AcceptMessage(const Message& message) override;
	
	bool MouseDown(const IPoint& mouse_pos) override;
	void MouseMove(const IPoint& mouse_pos) override;

	void KeyPressed(int keyCode) override;

	void OnAddAnimation(const void * pSender, AddAnimationArgs & args);

private:
	void Init();
	void UpdateAnimation();
	void FinishAnimation(BaseObject* object);
	void DeleteEmptyEffects();
	void FinishAllAnimations();
	
private:

	EffectsContainer _effectsContainer;

	struct EffectNode
	{
		ParticleEffectPtr eff;
		BaseObject* owner;
		EffectNode(ParticleEffectPtr pEff, BaseObject* pOwner)
		{
			eff = pEff;
			owner = pOwner;
		}
	};
	std::vector<EffectNode> _effects;

	Game* _game;	
};
