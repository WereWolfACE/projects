package GameElements.Characters.EvilGuys 
{
	import Constants.GameConstants;
	import Constants.GameEvents;
	import Constants.GameObjectsTypes;
	import flash.events.TimerEvent;
	import flash.utils.clearInterval;
	import flash.utils.setInterval;
	import flash.utils.Timer;
	import GameElements.Bullets.BulletBase;
	import GameElements.Characters.CharacterBase;
	import starling.events.Event;
	/**
	 * ...
	 * @author werewolf
	 */
	public class EvilGuyBase extends CharacterBase 
	{
		protected var _fireTimer:Timer;
		protected var _idInterval:uint;
		protected var _currentWeapon:int;
		
		protected var _framesForMove:int; //кол-во фреймов, через которое происходит движение
		protected var _framesCounter:int = 0; //счетчик фреймов
		
		public function EvilGuyBase() 
		{
			super();
			
			StarlingGlobalEventDispatcher.instance.dispatchEventWith(GameEvents.CHANGE_GAME_OBJECTS_COUNT, false, { key:Game.CHANGE_OBJECT_COUNT, action:Game.ADD, object:this } );
		
		}
		
		override protected function init():void 
		{							
			scaleX = scaleY = 0.5;
			_typeToCollision = GameObjectsTypes.EVIL_GUY;
			_currentState = GameConstants.MOVE;
			_normalState = GameConstants.MOVE;
			super.init();	
		}
		
		override public function set shotSpeed(value:int):void 
		{
			super.shotSpeed = value;
			if (_idInterval) clearInterval(_idInterval);
			_idInterval = setInterval(onFireTimer, shotSpeed);
		}
		
		public function get framesForMove():int 
		{
			return _framesForMove;
		}
		
		public function set framesForMove(value:int):void 
		{
			_framesForMove = value;
		}
		
		private function onFireTimer():void 
		{			
			var bullet:BulletBase = _weapon.fire(_currentWeapon, parent, x, realPosition.y);
			changeState(GameConstants.FIRE, backToNormalState);
		}
		
		override protected function addListeners():void 
		{
			addEventListener(Event.ENTER_FRAME, onEnterFrame);
		}
		
		override public function removeListeners():void 
		{
			super.removeListeners();
			removeEventListener(Event.ENTER_FRAME, onEnterFrame);
		}
		
		override public function destroy():void 
		{
			clearInterval(_idInterval);
			super.destroy();
		}
		
		private function onEnterFrame(e:Event):void 
		{		
			if(_framesCounter % _framesForMove == 0){
				y += _speed;
				if (y > StarlingMain.stage.stageHeight) {					
					StarlingGlobalEventDispatcher.instance.dispatchEventWith(GameEvents.CHANGE_GAME_OBJECTS_COUNT, false, { key:Game.CHANGE_OBJECT_COUNT, action:Game.REMOVE, object:this } );
					destroy();
				}
			}
			_framesCounter++;
		}

		override public function set hp(value:int):void 
		{
			super.hp = value;
			if (hp <= 0) {
				clearInterval(_idInterval);
			}
		}
		
		

	}

}