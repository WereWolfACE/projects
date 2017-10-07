package GameElements 
{
	import Constants.AnimationKeys;
	import flash.geom.Point;
	import starling.core.Starling;
	import starling.display.MovieClip;
	import starling.display.Sprite;
	import starling.events.Event;
	
	/**
	 * ...
	 * @author werewolf
	 */
	public class GameObject extends Sprite 
	{
		protected var _currentAnimation:MovieClip;
		protected var _animationKey:String;
		protected var _currentState:String;
		protected var _normalState:String; //постоянная анимация объекта(к которой всегда возвращаемся)
		protected var _typeToCollision:int;
		
		public function GameObject() 
		{
			touchable = false;
			init();			
			draw();
			addListeners();
		}
		
		protected function init():void {
			//virtual
		}
		
		protected function addListeners():void {
			//virtual
		}
		
		public function destroy():void {
			deleteAnimation();
			removeFromParent(true);			
		}
		
		public function draw():void {
			if (_animationKey == AnimationKeys.NONE) {
				trace("Не переопределено название спрайтлиста в дочернем классе");
				return;
			}
			deleteAnimation();
			_currentAnimation = AssetLib.createObject(_animationKey, _currentState);
			if (_currentAnimation) {				
				addChild(_currentAnimation);
				Starling.juggler.add(_currentAnimation);				
			}			
		}
		
		protected function deleteAnimation():void 
		{
			if (_currentAnimation) {
				Starling.juggler.remove(_currentAnimation);
				removeChild(_currentAnimation, true);				
			}
		}
		
		public function removeListeners():void {
			//virtual
		}
		
		//смена анимации
		public function changeState($state:String, $onCompleteFunction:Function = null):void {			
			_currentState = $state;
			draw();
			if(_currentAnimation && $onCompleteFunction != null){
				_currentAnimation.addEventListener(Event.COMPLETE, $onCompleteFunction);
			}			
		}
		
		public function get realPosition():Point 
		{
			return (new Point(x - width / 2, y - width / 2));
		}
		
		public function get typeToCollision():int 
		{
			return _typeToCollision;
		}
		
		

	}

}