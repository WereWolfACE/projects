#ifndef __GAME_H__
#define __GAME_H__

#pragma once
#include "Aim.h"
#include "CollisionGrid.h"
#include "Player.h"
#include "BaseTarget.h"
#include "BaseBullet.h"
#include "AddObjectArgs.h"
#include "DeleteObjectArgs.h"
#include "AddAnimationArgs.h"

class Game
{
public:
	Game(int width, int height);
	void Init();
	void Start();
	void Draw();
	void Update(float dt);
	bool IsGameEnd();
	void SetAnimationDelegate(std::shared_ptr<Delegate<AddAnimationArgs>> d);
	void OnMouseMove(const FPoint &mouse_pos);
	void OnMouseClick(const FPoint &mouse_pos);

	void OnDeleteObject(const void* pSender, DeleteObjectArgs &args);
	void OnAddObject(const void* pSender, AddObjectArgs &args);
	void OnAnimation(const void * pSender, AddAnimationArgs & args);

	void ChangeBulletBorderCollisionRule();
	void ChangeTargetTargetCollisionRule();
	void ChangePlayerView();
public:
	Event<AddAnimationArgs> OnAnimationEvent;
private:
	void ClearAll();
	void CreateAndPlacingObjects();
	void End(bool isWin = false);
	void DrawInfo();	
	void DrawResult();
	void LoadSettings();
	void CreateBorders();
	void CreateEventListeners();
	void StartGameEndTimer();
	void StartSecTimer();
	void UpdateTime();
	void DeleteElements();
	void CreateElements();
	bool CheckWin();
	int ParseString(std::string str);
	friend void GameEndTimerCallback(void* param);
	friend void SecTimerCallback(void* param);
private:
	static const int DEFAULT_COUNT_TARGET = 20;
	static const int DEFAULT_BULLET_SPEED = 30;
	static const int DEFAULT_GAME_TIME_IN_SECONDS = 50;
	static const int MIN_TARGET_SPEED = 80;
	static const int MAX_TARGET_SPEED = 200;

	int _countTarget = DEFAULT_COUNT_TARGET;
	int _bulletSpeed = DEFAULT_BULLET_SPEED;
	int _gameTimeInSeconds = DEFAULT_GAME_TIME_IN_SECONDS;	
	int _minAimSpeed = MIN_TARGET_SPEED;
	int _maxAimSpeed = MAX_TARGET_SPEED;

	int _fieldWidth;
	int _fieldHeight;
	//массив всех движущихс€ элементов
	std::vector<std::shared_ptr<MovedObject>> _movedObjects;	
	//границы игрового пол€
	std::vector<std::shared_ptr<Border>> _borders;
	//массив дл€ проверки столкновений
	std::shared_ptr<CollisionGrid> _grid;

	//массив элементов, которые должны удалитьс€
	std::vector<BaseObject*> _deletedMovedObjects;
	//массив элементов, которые должны создатьс€
	std::vector<std::shared_ptr<MovedObject>> _createdMovedObjects;

	int _killedEnemies;
	int _usedBullets;
	int _timeBetweenEnd;
	std::shared_ptr<Player> _player;	
	bool _isGameEnd;
	bool _isWin;

	int _gameTimer;
	int _secondsTimer;

	std::shared_ptr<Delegate<DeleteObjectArgs>> _deleteObjectDelegate;
	std::shared_ptr<Delegate<AddAnimationArgs>> _addAnimationDelegate;
	
	bool _bulletBorderCollisionChanged;
	bool _targetTargetCollisionChanged;	
};

#endif //__GAME_H__

