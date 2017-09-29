package GameElements.Characters 
{
	import Constants.AnimationKeys;
	import Constants.GameConstants;
	import Constants.GameEvents;
	import Constants.GameObjectsTypes;
	import flash.utils.clearInterval;
	import flash.utils.setInterval;
	import GameElements.Bullets.BulletBase;
	import GameElements.Weapon;
	import starling.events.EnterFrameEvent;
	import starling.events.Event;
	import starling.events.Touch;
	import starling.events.TouchEvent;
	import starling.events.TouchPhase;
	/**
	 * ...
	 * @author werewolf
	 */
	public class Hero extends CharacterBase 
	{
		private var _isFiring:Boolean;
		private var _idFireInterval:int;
		
		public function Hero() 
		{
			super();
		}
		
		override protected function init():void 
		{			
			_animationKey = AnimationKeys.HERO;
			_normalState = GameConstants.MOVE;
			_typeToCollision = GameObjectsTypes.HERO;
			scaleX = scaleY = 0.5;
			_hp = 100;
			_maxHp = _hp;
			_currentState = GameConstants.MOVE;
			super.init();
			touchable = true;
		}
		
		override protected function addListeners():void 
		{
			StarlingMain.stage.addEventListener(TouchEvent.TOUCH, onTouch);
		}
		
		override public function destroy():void 
		{			
			super.destroy();
			StarlingGlobalEventDispatcher.instance.dispatchEvent(new Event(GameEvents.EVENT_HERO_DESTROY));
		}
		
		override public function removeListeners():void 
		{
			super.removeListeners();
			StarlingMain.stage.removeEventListener(TouchEvent.TOUCH, onTouch);
		}
		
		private function onTouch(e:TouchEvent):void 
		{
			var touch:Touch = e.getTouch(stage);
			
			if (touch && (touch.phase == TouchPhase.HOVER || touch.phase == TouchPhase.MOVED)) {
				x = touch.globalX;
			}
			if (touch && touch.phase == TouchPhase.ENDED) {
				if(_isFiring){
					clearInterval(_idFireInterval);
					_isFiring = false;
				}
			}
			if (touch && touch.phase == TouchPhase.BEGAN) {
				_isFiring = true;
				_idFireInterval = setInterval(fire, GameConstants.HERO_MULTISHOT_SPEED);				
			}			
		}

		private function fire():void 
		{
			var bullet:BulletBase = _weapon.fire(Weapon.HERO_SIMPLE_BULLET, parent, x, realPosition.y);
			changeState(GameConstants.FIRE, backToNormalState);			
		}
		
		override public function set hp(value:int):void 
		{
			super.hp = value;
			if (_hp <= 0) {
				StarlingMain.stage.removeEventListener(TouchEvent.TOUCH, onTouch);
			}
		}
		
		
	}

}