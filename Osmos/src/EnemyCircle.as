package  
{
	import flash.geom.Point;
	/**
	 * ...
	 * @author Marshalova Valery
	 */
	public class EnemyCircle extends BaseCircle 
	{
		private var _playerRadius:Number;
		private var _color1:Array;
		private var _color2:Array;
		
		public function EnemyCircle(color1:Array, color2:Array, playerRadius:int) 
		{
			_playerRadius = playerRadius;
			_color1 = color1;
			_color2 = color2;
			//super.speed = Math.random() * (BaseCircle.MAX_SPEED - BaseCircle.MIN_SPEED) + BaseCircle.MIN_SPEED
		}
		
		public function update(radius:Number):void {
			_playerRadius = radius;
			calculateColor();
			draw();
		}
		
		private function calculateColor():void
		{			
			var i:int = Math.ceil((radius / ((playerRadius * 2) / 100)) / 10);
			
			if (i > 10)
				i = 10;
			
			var r3:int = _color1[0] - (_color1[0] - _color2[0]) / 10 * i; 
			var g3:int = _color1[1] - (_color1[1] - _color2[1]) / 10 * i;
			var b3:int = _color1[2] - (_color1[2] - _color2[2]) / 10 * i;
			color = (r3 << 16 | (g3 << 8) | b3 );
		}
		
		override public function set radius(value:Number):void 
		{
			super.radius = value;			
			update(_playerRadius);
		}
		
		public function get playerRadius():Number 
		{
			return _playerRadius;
		}
		
		public function set playerRadius(value:Number):void 
		{
			_playerRadius = value;
			update(_playerRadius);
		}
		
		public function setMoveVector(moveVector : Point):void {			
			var length : Number = Utils.getVectorLength(moveVector); 
			super.moveVector.x = moveVector.x / length;  
			super.moveVector.y = moveVector.y / length; 			
		}
		
	}

}