#include "stdafx.h"
#include "Game.h"
#include "Aim.h"
#include "Utils.h"
#include "Border.h"
#include "Cmath"
#include "DestroingBehaviour.h"
#include "ReflectionBehaviour.h"
#include "PlayerDefaultView.h"
#include "PlayerLeftView.h"


void GameEndTimerCallback(void* param)
{
	Game* game = static_cast<Game*>(param);
	game->_timeBetweenEnd = 0;
	game->End();
}

void SecTimerCallback(void* param)
{
	Game* game = static_cast<Game*>(param);
	game->UpdateTime();
	if (!game->IsGameEnd())
	{
		game->StartSecTimer();
	}
}

Game::Game(int width, int height)
{
	_fieldWidth = width;
	_fieldHeight = height;

	Init();
}

void Game::Init()
{	
	LoadSettings();	
}

void Game::LoadSettings()
{
	if (Core::fileSystem.FileExists("input.txt")) {
		IO::InputStreamPtr stream = Core::fileSystem.OpenRead("input.txt");
		IO::TextReader b(stream.get());
		std::string str = b.ReadUtf8Line();
		while (!str.empty())
		{
			int value = ParseString(str);
			if (value > 0) {
				if (str.find("CountTarget") != std::string::npos)
				{
					_countTarget = value;
				}
				else if (str.find("Speed") != std::string::npos)
				{
					_bulletSpeed = value;
				}
				else if (str.find("Time") != std::string::npos)
				{
					_gameTimeInSeconds = value;
				}
			}
			str = b.ReadUtf8Line();
		}
	}
}

void Game::CreateBorders()
{
	_borders.push_back(std::make_shared<Border>(FPoint(0, 0), FPoint(_fieldWidth, 0)));
	_borders.push_back(std::make_shared<Border>(FPoint(0, 0), FPoint(0, _fieldHeight)));
	_borders.push_back(std::make_shared<Border>(FPoint(_fieldWidth, 0), FPoint(_fieldWidth, _fieldHeight)));
	_borders.push_back(std::make_shared<Border>(FPoint(0, _fieldHeight), FPoint(_fieldWidth, _fieldHeight)));
}

void Game::CreateEventListeners()
{
	Delegate<AddObjectArgs> add = Delegate<AddObjectArgs>::Create<Game, &Game::OnAddObject>(this);
	_player->OnAddObjectsEvent.Add(add);

	_deleteObjectDelegate = std::make_shared<Delegate<DeleteObjectArgs>>(Delegate<DeleteObjectArgs>::Create<Game, &Game::OnDeleteObject>(this));
	_addAnimationDelegate = std::make_shared<Delegate<AddAnimationArgs>>(Delegate<AddAnimationArgs>::Create<Game, &Game::OnAnimation>(this));

}

void Game::StartGameEndTimer()
{
	_gameTimer = Core::timeMachine.doCallback(&GameEndTimerCallback, this, _gameTimeInSeconds * 1000);
}

void Game::StartSecTimer()
{
	_secondsTimer = Core::timeMachine.doCallback(&SecTimerCallback, this, 1000);
}

void Game::Start()
{		
	_isGameEnd = false;
	_isWin = false;
	_bulletBorderCollisionChanged = false;
	_targetTargetCollisionChanged = false;
	_killedEnemies = 0;
	_usedBullets = 0;
	_timeBetweenEnd = _gameTimeInSeconds;

	_grid = std::make_shared<CollisionGrid>();
	_player = std::make_shared<Player>();

	_player->SetLimitPosition(FPoint(0, _fieldWidth));
	_player->ChangeBulletSpeed(_bulletSpeed);

	CreateBorders();
	CreateEventListeners();	
	CreateAndPlacingObjects();
	StartGameEndTimer();
	StartSecTimer();
}



void Game::UpdateTime()
{
	_timeBetweenEnd--;
}

void Game::DeleteElements()
{
	std::vector<std::shared_ptr<MovedObject>>::iterator newEnd = _movedObjects.end();
	for (size_t i = 0; i < _deletedMovedObjects.size(); i++)
	{
		BaseObject* object = _deletedMovedObjects[i];
		newEnd = std::remove_if(_movedObjects.begin(), _movedObjects.end(), 
								[object](std::shared_ptr<MovedObject> const& obj) 
		{
			return obj.get() == object;
		});	
		if (newEnd != _movedObjects.end())
		{
			_movedObjects.erase(newEnd, _movedObjects.end());
		}
	}	
	_deletedMovedObjects.erase(_deletedMovedObjects.begin(), _deletedMovedObjects.end());
}

void Game::CreateElements()
{
	for (size_t i = 0; i < _createdMovedObjects.size(); i++)
	{		
		_movedObjects.push_back(_createdMovedObjects[i]);
		_createdMovedObjects[i]->SetLifeAnimation();
	}
	_createdMovedObjects.erase(_createdMovedObjects.begin(), _createdMovedObjects.end());
}

void Game::Update(float dt) 
{	
	if (!_isGameEnd) 
	{
		_grid->StartChecking(dt);
		for (size_t i = 0; i < _movedObjects.size(); i++) 
		{
			if (!_movedObjects[i]->CheckHitBorders(_borders, dt)) 
			{
				_movedObjects[i]->Move(dt);
			}
		}
		DeleteElements();
		CreateElements();

		if (CheckWin())
		{
			End(true);
		}
	}
}

bool Game::IsGameEnd()
{
	return _isGameEnd;
}

void Game::SetAnimationDelegate(std::shared_ptr<Delegate<AddAnimationArgs>> d)
{
	
}

bool Game::CheckWin()
{
	return _killedEnemies == _countTarget;
}
//создание и расстановка мишеней
void Game::CreateAndPlacingObjects()
{
	if (_isGameEnd)
	{
		return;
	}
	//массив пр€моугольников дл€ расстановки шаров
	std::vector<IRect> rectangles;

	int smallestWidth = _fieldWidth;
	int smallestHeight = _fieldHeight;
	int biggestWidth = -1;
	int biggestHeight = -1;
	for (int i = 0; i < _countTarget; i++) 
	{		
		std::shared_ptr<MovedObject> enemy = std::make_shared<Aim>(-1, -1);
		float randX = Utils::GetInstance().RandWithNegative(_fieldWidth);
		float randY = Utils::GetInstance().RandWithNegative(_fieldHeight);
		FPoint vector = Utils::GetInstance().GetVector(FPoint(randX, randY), enemy->GetCenter());
		enemy->SetMoveVector(vector);
		
		enemy->SetSpeed(Utils::GetInstance().RandRange(_minAimSpeed, _maxAimSpeed));
		enemy->OnDeleteObjectEvent.Add(*_deleteObjectDelegate);
		enemy->OnAnimationEvent.Add(*_addAnimationDelegate);
		_movedObjects.push_back(enemy);

		smallestWidth = (enemy->Width() < smallestWidth) ? enemy->Width() : smallestWidth;
		smallestHeight = (enemy->Height() < smallestHeight) ? enemy->Height() : smallestHeight;
		biggestWidth = (enemy->Width() > biggestWidth) ? enemy->Width() : biggestWidth;
		biggestHeight = (enemy->Height() > biggestHeight) ? enemy->Height() : biggestHeight;

	}
		
	std::sort(_movedObjects.begin(), _movedObjects.end());

	//размер €чейки грида дл€ расстановки равен размеру самого маленького объекта 
	int gridWidth = smallestWidth;
	int gridHeight = smallestHeight;

	int colsCount = math::floor(_fieldWidth / gridWidth);
	int rowsCount = math::floor(_fieldHeight / gridHeight);
	
	for (int i = 0; i < colsCount; i++) 
	{
		for (int j = 0; j < rowsCount; j++) 
		{
			rectangles.push_back(IRect(
				i * gridWidth,
				j * gridHeight,
				gridWidth,
				gridHeight
				));
		}
	}

	bool isPlaceable = true;
	for (size_t i = 0; i < _movedObjects.size(); i++)
	{
		if (rectangles.size() > 0)
		{
			std::shared_ptr<MovedObject> object = _movedObjects[i];			
			if (object->IsPlaced()) continue;
			isPlaceable = true;
			int index = math::floor(rand() % rectangles.size());
			float xCoord = rectangles[index].x;
			float yCoord = rectangles[index].y;
			object->SetPosition(xCoord, yCoord);

			for (size_t j = 0; j < i; j++)
			{
				std::shared_ptr<MovedObject> placedObject = _movedObjects[j];				
				if (!placedObject->IsPlaced()) continue;
				if (object->Intersects(placedObject))
				{
					isPlaceable = false;
					object->SetPosition(-1, -1);
					break;
				}				
			}

			if (!isPlaceable)
			{
				//если текущий объект не подходит по размерам в выбранную €чейку, 
				//берем объект меньшего размера

				for (size_t k = i + 1; k < _movedObjects.size(); k++) {
					std::shared_ptr<MovedObject> smallestObject = _movedObjects[k];
					isPlaceable = true;
					if (smallestObject->IsPlaced()) continue;

					for (size_t j = 0; j < i; j++)
					{
						std::shared_ptr<MovedObject> placedObject = _movedObjects[j];
						if (!placedObject->IsPlaced()) continue;
						smallestObject->SetPosition(xCoord, yCoord);
						if (smallestObject->Intersects(placedObject))
						{
							isPlaceable = false;
							smallestObject->SetPosition(-1, -1);
							break;
						}
					}
					if (isPlaceable)
					{
						_movedObjects[i] = smallestObject;
						_movedObjects.erase(_movedObjects.begin() + k);
						_movedObjects.insert(_movedObjects.begin() + i + 1, object);
						break;
					}
				}
				//если в выбранную €чейку не поставили никакого объекта, 
				//то остаемс€ на том же элементе массива
				if (!isPlaceable) {
					i--;
				}
			}
			//выбранна€ €чейка больше не будет использоватьс€ в расстановке
			rectangles.erase(rectangles.begin() + index);
		}		
		else {
			//если шаров намного больше, чем свободного места, удалем лишние шары
			_movedObjects.resize(i + 1);
			break;
		}

	}
	_grid->Create(_fieldWidth, _fieldHeight, biggestWidth, biggestHeight);
	_grid->SetObjects(&_movedObjects);
}

void Game::Draw()
{
	if (!_isGameEnd)
	{
		for (size_t i = 0; i < _borders.size(); i++)
		{
			_borders[i]->Draw();
		}
		for (size_t i = 0; i < _movedObjects.size(); i++)
		{
			_movedObjects[i]->Draw();
		}
		_player->Draw();
		DrawInfo();
	}
	else
	{
		DrawResult();
	}
	
}

void Game::DrawInfo()
{
	if (!Render::isFontLoaded("arial"))
		return;

	Render::BindFont("arial");

	int dy = Render::getFontHeight();
	int x = 10;
	int y = Render::device.Height() - 20;

	Render::PrintString(x, y, std::string("Time: ") + utils::lexical_cast(_timeBetweenEnd) + std::string(" sec"), 1.0f, LeftAlign, BottomAlign);
	Render::PrintString(x, y -= dy, std::string("Destroyed targets: ") + utils::lexical_cast(_killedEnemies) , 1.0f, LeftAlign, BottomAlign);
	Render::PrintString(x, y -= dy, std::string("Used bullets: ") + utils::lexical_cast(_usedBullets), 1.0f, LeftAlign, BottomAlign);

	x = Render::device.Width() / 2;
	y = Render::device.Height() - 20;
	Render::PrintString(x, y, std::string("Settings"), 1.0f, CenterAlign, BottomAlign);
	Render::PrintString(x, y -= dy, std::string("A - change collision rule for bullets and borders"), 1.0f, CenterAlign, BottomAlign);
	Render::PrintString(x, y -= dy, std::string("W - change collision rule for targets"), 1.0f, CenterAlign, BottomAlign);
	Render::PrintString(x, y -= dy, std::string("S - change weapon view"), 1.0f, CenterAlign, BottomAlign);
}

void Game::OnMouseMove(const FPoint &mouse_pos)
{	
	if (!_isGameEnd) 
	{
		_player->MoveTo(mouse_pos);
	}
}

void Game::OnMouseClick(const FPoint &mouse_pos)
{
	if (!_isGameEnd) 
	{
		_player->Fire(mouse_pos);
	}	
}

void Game::OnDeleteObject(const void * pSender, DeleteObjectArgs & args)
{
	BaseObject* object = args.GetChangedObject();
	BaseTarget* target = dynamic_cast<BaseTarget*>(object);
	if (target)
	{
		_killedEnemies++;
	}

	object->DestroyAnimation();
	_deletedMovedObjects.push_back(object);	
}

void Game::OnAddObject(const void * pSender, AddObjectArgs & args)
{	
	auto object = args.GetChangedObject().lock();
	if (object) 
	{		
		auto movedObject = dynamic_pointer_cast<MovedObject>(object);
		if (movedObject) 
		{
			std::shared_ptr<MovedObject> mo = movedObject;
			auto bullet = dynamic_pointer_cast<BaseBullet>(movedObject);
			if (bullet)
			{
				_usedBullets++;
			}
			_createdMovedObjects.push_back(mo);
			mo->OnDeleteObjectEvent.Add(*_deleteObjectDelegate);
			mo->OnAnimationEvent.Add(*_addAnimationDelegate);
			mo->CreateAnimation();
		}
	}
}

void Game::OnAnimation(const void * pSender, AddAnimationArgs & args)
{
	if (!_isGameEnd)
	{
		OnAnimationEvent.Invoke(pSender, args);
	}
}

void Game::ChangeBulletBorderCollisionRule()
{	
	if (_isGameEnd)
	{
		return;
	}
	std::shared_ptr<IBehaviour> shared;
	if (_bulletBorderCollisionChanged) 
	{
		shared = std::make_shared<DestroingBehaviour>();
	}
	else
	{
		shared = std::make_shared<ReflectionBehaviour>();
	}
	std::for_each(_movedObjects.begin(), _movedObjects.end(), 
				[shared](std::shared_ptr<MovedObject> const& obj) 
	{ 
		auto bullet = dynamic_pointer_cast<BaseBullet>(obj);
		if (bullet) 
		{
			obj->ChangeBorderCollisionBehaviour(shared);
		}
	});
	_player->ChangeBulletCollisionBehaviour(nullptr, shared, nullptr);
	_bulletBorderCollisionChanged = !_bulletBorderCollisionChanged;
}

void Game::ChangeTargetTargetCollisionRule()
{
	if (_isGameEnd)
	{
		return;
	}
	std::shared_ptr<IBehaviour> shared;
	if (_targetTargetCollisionChanged)
	{
		shared = std::make_shared<ReflectionBehaviour>();		
	}
	else
	{
		shared = nullptr;
	}
	std::for_each(_movedObjects.begin(), _movedObjects.end(),
		[shared](std::shared_ptr<MovedObject> const& obj)
	{
		auto target = dynamic_pointer_cast<BaseTarget>(obj);
		if (target)
		{
			obj->ChangeTargetCollisionBehaviour(shared);
		}
	});
	_targetTargetCollisionChanged = !_targetTargetCollisionChanged;
}

void Game::ChangePlayerView()
{	
	if (_isGameEnd)
	{
		return;
	}
	_player->ChangeView();
}

int Game::ParseString(std::string str)
{
	size_t digits = str.find_first_of("1234567890");
	if (digits != std::string::npos)
	{
		return atoi(str.c_str() + digits);
	}
	return 0;
}

void Game::End(bool isWin)
{
	_isGameEnd = true;
	_isWin = isWin;
	ClearAll();	
}

void Game::ClearAll()
{
	Core::timeMachine.cancelJob(_gameTimer);
	Core::timeMachine.cancelJob(_secondsTimer);

	_movedObjects.erase(_movedObjects.begin(), _movedObjects.end());
	_deletedMovedObjects.erase(_deletedMovedObjects.begin(), _deletedMovedObjects.end());
	_createdMovedObjects.erase(_createdMovedObjects.begin(), _createdMovedObjects.end());
	
	_deleteObjectDelegate.reset();
	_addAnimationDelegate.reset();	
	_grid.reset();
	_player.reset();	
}

void Game::DrawResult()
{
	if (!Render::isFontLoaded("arial"))
		return;

	Render::BindFont("arial");

	int dy = Render::getFontHeight();
	int x = Render::device.Width() / 2;
	int y = Render::device.Height() / 2 + 3 * dy;

	if (_isWin) 
	{
		Render::PrintString(x, y, std::string("GREAT! EXCELLENT! HEAVENLY!"), 1.0f, CenterAlign, CenterAlign);
		Render::PrintString(x, y -= dy, std::string("Time remain: ") + utils::lexical_cast(_timeBetweenEnd) + std::string(" sec"), 1.0f, CenterAlign, CenterAlign);
	}
	else
	{
		Render::PrintString(x, y, std::string("CHEER UP! KEEP IN THERE!"), 1.0f, CenterAlign, CenterAlign);
		Render::PrintString(x, y -= dy, std::string("Targets remain: ") + utils::lexical_cast(_countTarget - _killedEnemies), 1.0f, CenterAlign, CenterAlign);
	}
	Render::PrintString(x, y -= dy, std::string("Targets killed: ") + utils::lexical_cast(_killedEnemies), 1.0f, CenterAlign, CenterAlign);
	Render::PrintString(x, y -= dy, std::string("Used bullets: ") + utils::lexical_cast(_usedBullets), 1.0f, CenterAlign, CenterAlign);
	Render::PrintString(x, y -= (2 * dy), std::string("Press Q to new game"), 1.0f, CenterAlign, CenterAlign);

}
