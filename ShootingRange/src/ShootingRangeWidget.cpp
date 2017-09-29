#include "stdafx.h"
#include "ShootingRangeWidget.h"

ShootingRangeWidget::ShootingRangeWidget(const std::string& name, rapidxml::xml_node<>* elem)
	: Widget(name)	
{
	Init();
}



void ShootingRangeWidget::Init()
{
	int width, height;
	Delegate<AddAnimationArgs> d = Delegate<AddAnimationArgs>::Create<ShootingRangeWidget, &ShootingRangeWidget::OnAddAnimation>(this);
	Core::appDelegate->GameContentSize(0, 0, width, height);
	ShowCursor(FALSE);
	_game = new Game(width, height - 100);
	_game->OnAnimationEvent.Add(d);
	_game->Start();
}

void ShootingRangeWidget::UpdateAnimation()
{
	std::for_each(_effects.begin(), _effects.end(),
		[](EffectNode const& obj)
	{	
		if (obj.eff != NULL) {
			FPoint point = obj.owner->GetCenter();
			obj.eff->SetPos(point);
		}
	});
}

void ShootingRangeWidget::OnAddAnimation(const void * pSender, AddAnimationArgs & args)
{
	AnimationType type = args.GetType();
	std::string name = args.GetName();
	FPoint position = args.GetPosition();
	BaseObject* owner = (BaseObject*)pSender;
	switch (type)
	{
		case AnimationType::ANIMATION_DELETE:
		{
			FinishAnimation(owner);
			ParticleEffectPtr eff = _effectsContainer.AddEffect(name);
			eff->SetPos(position);
			eff->Reset();
			break;
		}
		case AnimationType::ANIMATION_SET_UNLIMITED:
		{
			ParticleEffectPtr eff = _effectsContainer.AddEffect(name);
			eff->SetPos(position);
			eff->Reset();			
			_effects.push_back(EffectNode(eff, owner));
			break;
		}
		default:
		{
			ParticleEffectPtr eff = _effectsContainer.AddEffect(name);
			eff->SetPos(position);
			eff->Reset();
			break;
		}
	}
}

void ShootingRangeWidget::FinishAnimation(BaseObject* object)
{	
	std::for_each(_effects.begin(), _effects.end(),
		[object](EffectNode & obj)
	{			
		if (obj.owner == object && obj.eff != NULL)			
		{
			obj.eff->Kill();
			obj.eff = NULL;
		}		
	});	
}

void ShootingRangeWidget::FinishAllAnimations()
{
	std::for_each(_effects.begin(), _effects.end(),
		[](EffectNode & obj)
	{	
		if (obj.eff != NULL) 
		{
			obj.eff->Kill();
			obj.eff = NULL;
		}
	});
}

void ShootingRangeWidget::DeleteEmptyEffects()
{
	auto newEnd = std::remove_if(_effects.begin(), _effects.end(),
		[](EffectNode const& obj)
	{
		return obj.eff == NULL;
	});
	if (newEnd != _effects.end())
	{
		_effects.erase(newEnd, _effects.end());
	}
}

void ShootingRangeWidget::Draw()
{
	_game->Draw();
	_effectsContainer.Draw();
}

void ShootingRangeWidget::Update(float dt)
{	
	_game->Update(dt);	
	if (!_game->IsGameEnd()) 
	{
		UpdateAnimation();
	}
	else
	{
		FinishAllAnimations();
	}
	DeleteEmptyEffects();
	_effectsContainer.Update(dt);	
}

bool ShootingRangeWidget::MouseDown(const IPoint &mouse_pos)
{
	if (Core::mainInput.GetMouseLeftButton())
	{
		_game->OnMouseClick(mouse_pos);
	}	
	return false;
}

void ShootingRangeWidget::MouseMove(const IPoint &mouse_pos)
{
	_game->OnMouseMove(mouse_pos);
}

void ShootingRangeWidget::AcceptMessage(const Message& message)
{
	//
	// Виджету могут посылаться сообщения с параметрами.
	//

	const std::string& publisher = message.getPublisher();
	const std::string& data = message.getData();
}

void ShootingRangeWidget::KeyPressed(int keyCode)
{
	if (keyCode == VK_A) {
		//пули отлетают от стен
		_game->ChangeBulletBorderCollisionRule();
	}
	if (keyCode == VK_W) {
		// шары перестают сталкиваться
		_game->ChangeTargetTargetCollisionRule();
	}
	if (keyCode == VK_S) {
		// поменять внешний вид пушки
		_game->ChangePlayerView();
	}
	if (keyCode == VK_Q) {
		// новая игра		
		if (_game->IsGameEnd())
		{
			_game->Start();
		}
	}
}
