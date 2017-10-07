
package
{
	import Constants.GameConstants;
	import Constants.GameEvents;
	import Constants.GameObjectsTypes;
	import flash.geom.Rectangle;
	import flash.ui.Mouse;
	import flash.utils.clearInterval;
	import flash.utils.clearTimeout;
	import flash.utils.setInterval;
	import flash.utils.setTimeout;
	import GameElements.Bullets.BulletBase;
	import GameElements.Characters.CharacterBase;
	import GameElements.Characters.CharactersCreator;
	import GameElements.Characters.EvilGuys.EvilGuyBase;
	import GameElements.GameObject;
	import starling.display.DisplayObject;
	import starling.display.Sprite;
	import starling.display.Stage;
	import starling.events.Event;
	import starling.events.Touch;
	import starling.events.TouchEvent;
	import starling.events.TouchPhase;
	import starling.text.TextField;
	import starling.utils.HAlign;
	import starling.utils.VAlign;
	
	/**
	 * ...
	 * @author werewolf
	 */
	public class Game extends Sprite
	{
		static private var _characterCreator:CharactersCreator;
		private static var _evilGuysBullets:Vector.<GameObject>;
		private static var _heroBullets:Vector.<GameObject>;		
		private var _evilGuys:Vector.<GameObject>;
		private var _hero:GameObject;
		
		private var _idCreateEnemyTimeout:int;
		private var _idCheckHitInterval:int;
		
		private var _killedEvilGuysCount:int = 0;
		
		private static var _instance:Game;
		
		//массив для проверки столкновений
		private var _grid : CollisionGrid;
		
		static public const CHANGE_BULLET_COUNT:int = 1;
		static public const CHANGE_OBJECT_COUNT:int = 2;
		
		static public const REMOVE:String = "remove";
		static public const ADD:String = "add";
		
		private var tfBullet:TextField;
		private var tfObject:TextField;
		
		public function Game()
		{
			if (stage)
				init();
			else
				addEventListener(Event.ADDED_TO_STAGE, init);
		}
		
		public function init():void
		{
			removeEventListener(Event.ADDED_TO_STAGE, init);					
		}
		
		//вывод на экран количества объектов и пуль
		private function drawStats():void 
		{
			var text:String = "bullets: " + (_evilGuysBullets.length + _heroBullets.length).toString();
			tfBullet = GameUtils.createTextField(text, 9, 0xffffff, true, "center", "center", 50);
			tfBullet.x = 0;
			tfBullet.y = 50;
			addChild(tfBullet);
			text = "objects: " + (_evilGuys.length + 1).toString();
			tfObject = GameUtils.createTextField(text, 9, 0xffffff, true, "center", "center", 50);
			tfObject.x = 0;
			tfObject.y = 70;
			addChild(tfObject);
		}
		
		public function start():void
		{
			_grid = new CollisionGrid();
			StarlingGlobalEventDispatcher.instance.addEventListener(GameEvents.COLLISION, onCollision);
			_evilGuys = new Vector.<GameObject>();
			_evilGuysBullets = new Vector.<GameObject>();
			_heroBullets = new Vector.<GameObject>();
			
			_characterCreator = new CharactersCreator();
			
			drawStats();
			StarlingGlobalEventDispatcher.instance.addEventListener(GameEvents.CHANGE_GAME_OBJECTS_COUNT, onChangeInfo);
			
			_hero = _characterCreator.create(CharactersCreator.HERO, this, stage.stageWidth / 2, stage.stageHeight);
			_hero.y = stage.stageHeight - _hero.height / 2;
			StarlingGlobalEventDispatcher.instance.addEventListener(GameEvents.EVENT_HERO_DESTROY, onHeroDestroy);
			_idCreateEnemyTimeout = setTimeout(onCreateEnemyTimeout, 10);
	
			_idCheckHitInterval = setInterval(onCheckHit, 100);
		}
		
		private function onCheckHit():void 
		{
			_grid.startChecking();
		}

		private function onCollision(e:Event):void 
		{
			var object1:GameObject = e.data.object1;
			var object2:GameObject = e.data.object2;
			
			if (object1 is CharacterBase && object2 is CharacterBase) {
				StarlingGlobalEventDispatcher.instance.dispatchEventWith(GameEvents.CHANGE_GAME_OBJECTS_COUNT, false, { key:Game.CHANGE_OBJECT_COUNT, action:Game.REMOVE, object:object1, visible:true } );
				StarlingGlobalEventDispatcher.instance.dispatchEventWith(GameEvents.CHANGE_GAME_OBJECTS_COUNT, false, { key:Game.CHANGE_OBJECT_COUNT, action:Game.REMOVE, object:object2, visible:true } );
				return;
			}
			var character:CharacterBase = (object1 is CharacterBase) ? (object1 as CharacterBase) : (object2 as CharacterBase);
			var bullet:BulletBase = (object1 is BulletBase) ? (object1 as BulletBase) : (object2 as BulletBase);
			if(character && bullet){
				character.hp -= bullet.damage;
				if (character.hp <= 0) {
					StarlingGlobalEventDispatcher.instance.dispatchEventWith(GameEvents.CHANGE_GAME_OBJECTS_COUNT, false, { key:Game.CHANGE_OBJECT_COUNT, action:Game.REMOVE, object:character, visible:true } );
				}
				StarlingGlobalEventDispatcher.instance.dispatchEventWith(GameEvents.CHANGE_GAME_OBJECTS_COUNT, false, { key:Game.CHANGE_BULLET_COUNT, action:Game.REMOVE, object:bullet, visible:true } );
			}			
		}
		
		//обработчик события изменения количества объектов на поле
		private function onChangeInfo(e:Event):void 
		{
			//Пример: { key:GameEvents.CHANGE_BULLET_COUNT, action:Game.instance.ADD, object:bullet }
			var object:GameObject = e.data.object;
			var action:String = e.data.action;
			var key:int = e.data.key;
			var visible:Boolean = e.data.visible ? e.data.visible : false;

			if (key == CHANGE_BULLET_COUNT) {				
				if (action == ADD) {
					if (object.typeToCollision == GameObjectsTypes.EVIL_BULLET) {
						_evilGuysBullets.push(object);
					}else if (object.typeToCollision == GameObjectsTypes.HERO_BULLET) {
						_heroBullets.push(object);
					}
				}else {
					var index:int;
					if (object.typeToCollision == GameObjectsTypes.EVIL_BULLET) {
						index = _evilGuysBullets.indexOf(object);
						if (index > -1) {
							if(visible){
								_evilGuysBullets[index].removeListeners();	
								_evilGuysBullets[index].changeState(GameConstants.HIT, _evilGuysBullets[index].destroy);
							}else {
								_evilGuysBullets[index].destroy();	
							}
							_evilGuysBullets.splice(index, 1);
						}
					}else if (object.typeToCollision == GameObjectsTypes.HERO_BULLET) {
						index = _heroBullets.indexOf(object);
						if (index > -1) {
							if(visible){
								_heroBullets[index].removeListeners();
								_heroBullets[index].changeState(GameConstants.HIT, _heroBullets[index].destroy);
							}else {
								_heroBullets[index].destroy();	
							}
							_heroBullets.splice(index, 1);
						}
					}
				}
				
				var x:int = _evilGuysBullets.length + _heroBullets.length;
				tfBullet.text = "bullets: " + x.toString();
				
			}else if (key == CHANGE_OBJECT_COUNT) {				
				if (action == ADD) {
					if (object.typeToCollision == GameObjectsTypes.EVIL_GUY) {
						_evilGuys.push(object);
					}
				}else {
					if (object.typeToCollision == GameObjectsTypes.EVIL_GUY) {
						index = _evilGuys.indexOf(object);
						if (index > -1) {
							if(visible){
								_evilGuys[index].changeState(GameConstants.DESTROY, _evilGuys[index].destroy);
								_killedEvilGuysCount++;
								_hero.changeState(GameConstants.IDLE, (_hero as CharacterBase).backToNormalState);
							}else {
								_evilGuys[index].destroy();
							}
							_evilGuys.splice(index, 1);							
						}
					}else if (object.typeToCollision == GameObjectsTypes.HERO) {
						_hero.changeState(GameConstants.DESTROY, _hero.destroy);
					}
				}
				
				x = _evilGuys.length + 1;
				tfObject.text = "objects: " + x.toString();
			}
			_grid.refresh(_evilGuysBullets, _heroBullets, _evilGuys, _hero);
		}
		
		private function onHeroDestroy(e:Event):void
		{
			StarlingGlobalEventDispatcher.instance.removeEventListener(GameEvents.EVENT_HERO_DESTROY, onHeroDestroy);
			stopGame();
			Mouse.show();
			showGameEndText();
		}
		
		//окно конца игры
		private function showGameEndText():void
		{			
			var text:String = "KILLED ENEMIES: " + _killedEvilGuysCount + "\nGAME OVER...";
			var tf:TextField = GameUtils.createTextField(text, 24, 0xff0000, true, "center", "center", 300, 100);
			tf.x = stage.stageWidth / 2 - tf.width / 2;
			tf.y = stage.stageHeight / 2 - tf.height / 2;
			addChild(tf);
			
			var sprite:Sprite = new Sprite();
			var tf2:TextField = GameUtils.createTextField("RESTART", 16, 0x00ff00, true, VAlign.CENTER, HAlign.CENTER, 100, 30);
			tf2.border = true;
			sprite.touchable = true;
			sprite.addChild(tf2);
			sprite.addEventListener(TouchEvent.TOUCH, onRestartTouch);
			sprite.x = stage.stageWidth / 2 - sprite.width / 2;
			sprite.y = tf.y + tf.height + 10;						
			addChild(sprite);
		}
		
		private function onRestartTouch(e:TouchEvent):void 
		{
			var touch:Touch = e.getTouch(stage);
			
			if (touch && touch.phase == TouchPhase.ENDED) {
				e.target.removeEventListener(TouchEvent.TOUCH, onRestartTouch);
				removeChildren(0, -1, true);
				Mouse.hide();
				start();
			}
		}
		
		//создание врагов по таймеру
		private function onCreateEnemyTimeout():void
		{
			clearTimeout(_idCreateEnemyTimeout);
			if (_evilGuys.length < GameConstants.COUNT_ENEMIES)
			{	
				var x:int = getXForEnemy();
				var y:int = 40;
				var type:int = getEnemyTypeByHeroLevel();
				var enemy:CharacterBase = characterCreator.create(type, this, x, y);
				(enemy as EvilGuyBase).framesForMove = GameUtils.randRange(15, 20);
				enemy.speed = GameUtils.randRange(1, 2);				
			}
			var delay:Number = getDelayForEnemyCreating();
			_idCreateEnemyTimeout = setTimeout(onCreateEnemyTimeout, delay);
		}
		
		//появление определенного типа врагов в зависимости от уровня героя
		private function getEnemyTypeByHeroLevel():int
		{
			var type:int;
			if (_killedEvilGuysCount < GameConstants.COUNT_KILLED_FOR_MIDDLE_SPEED)
			{
				type = CharactersCreator.SLOW_EVIL_GUY;
			}
			else if (_killedEvilGuysCount < GameConstants.COUNT_KILLED_FOR_FAST_SPEED)
			{
				type = CharactersCreator.MIDDLE_EVIL_GUY;
			}
			else
			{
				type = CharactersCreator.FAST_EVIL_GUY;
			}
			return type;			
		}
		
		private function getXForEnemy():int
		{
			return GameUtils.randRange(100, stage.stageWidth - GameConstants.CHARACTER_WIDTH);
		}
		
		private function getDelayForEnemyCreating():Number
		{
			return GameUtils.randRange(GameConstants.MIN_ENEMY_CREATE_TIME, GameConstants.MAX_ENEMY_CREATE_TIME);
		}

		private function stopGame():void
		{			
			StarlingGlobalEventDispatcher.instance.removeEventListener(GameEvents.COLLISION, onCollision);			
			StarlingGlobalEventDispatcher.instance.removeEventListener(GameEvents.CHANGE_GAME_OBJECTS_COUNT, onChangeInfo);
			
			_killedEvilGuysCount = 0;
			clearTimeout(_idCreateEnemyTimeout);
			clearInterval(_idCheckHitInterval);
			for (var i:int = 0; i < _evilGuys.length; i++)
			{
				_evilGuys[i].destroy();
			}
			for (i = 0; i < _evilGuysBullets.length; i++)
			{
				_evilGuysBullets[i].destroy();
			}
			for (i = 0; i < _heroBullets.length; i++)
			{
				_heroBullets[i].destroy();
			}
			_hero = null;
			_evilGuys = null;
			_evilGuysBullets = null;
			_heroBullets = null;
		}
		
		static public function get characterCreator():CharactersCreator
		{
			return _characterCreator;
		}
		
		static public function get evilGuysBullets():Vector.<GameObject>
		{
			return _evilGuysBullets;
		}
		
		static public function set evilGuysBullets(value:Vector.<GameObject>):void
		{
			_evilGuysBullets = value;
		}
		
		static public function get heroBullets():Vector.<GameObject>
		{
			return _heroBullets;
		}
		
		static public function set heroBullets(value:Vector.<GameObject>):void
		{
			_heroBullets = value;
		}
		
		static public function get instance():Game
		{
			if (!_instance)
				_instance = new Game();
			return _instance;
		}
	
	}

}