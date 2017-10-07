package GameElements.Characters 
{
	import Constants.GameConstants;
	import GameElements.GameObject;
	import GameElements.Weapon;
	import starling.display.Quad;
	import starling.display.Sprite;
	/**
	 * ...
	 * @author werewolf
	 */
	public class CharacterBase extends GameObject
	{
		protected var _weapon:Weapon;
		protected var _hp:int;
		protected var _maxHp:int;
		protected var _speed:Number;
		protected var _shotSpeed:int;
		
		protected var _lifeProgress:Sprite;
		
		public function CharacterBase() {			
			super();
		}
		
		override protected function init():void {
			_weapon = new Weapon();
		}
		
		//возврат в обычную анимацию
		public function backToNormalState():void {
			changeState(_normalState);
		}

		override public function draw():void 
		{
			super.draw();
			if (_currentState == GameConstants.DESTROY) {
				if (contains(_lifeProgress)) removeChild(_lifeProgress, true);
				return;
			}
			drawLife();
		}
		
		private function drawLife():void 
		{
			var lifeWidth:int = width / _maxHp * _hp;
			if (!_lifeProgress) {
				_lifeProgress = new Sprite();
				var quadWidth:int = lifeWidth;
				var color:int = 0x00FF00;
				var quad:Quad = new Quad(width, 5, color);
				_lifeProgress.addChild(quad);
				_lifeProgress.x = -width / 2;
				_lifeProgress.y = y - height / 2 - 5;							
				addChild(_lifeProgress);
			}else {				
				_lifeProgress.width = lifeWidth; 
				setChildIndex(_lifeProgress, numChildren - 1);
			}
		}
				
		public function get hp():int 
		{
			return _hp;
		}
		
		public function set hp(value:int):void 
		{
			if (_hp <= 0) return;
			_hp = value;
			drawLife();
		}
		
		public function get speed():Number 
		{
			return _speed;
		}
		
		public function set speed(value:Number):void 
		{
			_speed = value;
		}
		
		public function get shotSpeed():int 
		{
			return _shotSpeed;
		}
		
		public function set shotSpeed(value:int):void 
		{
			_shotSpeed = value;
		}

		override public function changeState($state:String, $onCompleteFunction:Function = null):void 
		{
			if (hp <= 0 && _currentState == GameConstants.DESTROY) return;
			super.changeState($state, $onCompleteFunction);
		}
		
		override public function get width():Number 
		{
			return GameConstants.CHARACTER_WIDTH;
		}

		override public function get height():Number 
		{
			return GameConstants.CHARACTER_HEIGHT;
		}
	}

}