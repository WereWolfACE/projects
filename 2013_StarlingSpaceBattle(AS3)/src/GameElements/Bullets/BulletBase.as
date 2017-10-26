package GameElements.Bullets 
{
	import Constants.AnimationKeys;
	import Constants.GameConstants;
	import Constants.GameEvents;
	import GameElements.GameObject;
	import starling.core.Starling;
	import starling.display.MovieClip;
	import starling.display.Sprite;
	import starling.events.Event;
	/**
	 * Патрон
	 * @author werewolf
	 */
	public class BulletBase extends GameObject
	{		
		protected var _speed:int;
		protected var _damage:int;		
		
		public function BulletBase() 
		{
			_animationKey = AnimationKeys.NONE;
			_currentState = GameConstants.MOVE;
			super();
			
			StarlingGlobalEventDispatcher.instance.dispatchEventWith(GameEvents.CHANGE_GAME_OBJECTS_COUNT, false, { key:Game.CHANGE_BULLET_COUNT, action:Game.ADD, object:this } );
		
		}
		
		override protected function addListeners():void {
			addEventListener(Event.ENTER_FRAME, onEnterFrame);
		}
		
		protected function onEnterFrame(e:Event):void 
		{
			y += speed;
			if (y > StarlingMain.stage.stageHeight || y < height) {
				StarlingGlobalEventDispatcher.instance.dispatchEventWith(GameEvents.CHANGE_GAME_OBJECTS_COUNT, false, { key:Game.CHANGE_BULLET_COUNT, action:Game.REMOVE, object:this } );
			
				destroy();
			}
		}		

		override public function removeListeners():void {
			removeEventListener(Event.ENTER_FRAME, onEnterFrame);
		}
		
		public function get speed():int 
		{
			return _speed;
		}
		
		public function get damage():int 
		{
			return _damage;
		}
		
		override public function get width():Number 
		{
			return GameConstants.BULLET_WIDTH;
		}
		
		override public function get height():Number 
		{
			return GameConstants.BULLET_HEIGHT;
		}
	}
}