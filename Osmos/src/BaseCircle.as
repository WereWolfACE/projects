package  
{
	import flash.display.Sprite;
	import flash.geom.Point;
	/**
	 * ...
	 * @author Marshalova Valery
	 */
	public class BaseCircle
	{
		public static const MAX_SPEED : int = 1200;
		public static const MIN_SPEED : int = 10;
		public static const MAX_TIME : int = 4000;
		
		private var _radius : Number;
		private var _color : int;
		private var _position : Point;
		private var _graphics : Graphics;
		
		private var _moveVector : Point;
		private var _isMoved : Boolean;
		private var _speed : Number;
		
		public function BaseCircle(radius : int = 20) 
		{
			_radius = radius;
			_color = 0x333333;
			_position = new Point(-1, -1);
			_graphics = new Graphics();
			
			_isMoved = false;
			_moveVector = new Point();	
			_speed = 0;
		}
		
		public function get radius():Number 
		{
			return _radius;
		}
		
		public function set radius(value:Number):void 
		{
			_radius = value;
		}
		
		public function get color():int 
		{
			return _color;
		}
		
		public function set color(value:int):void 
		{
			_color = value;
		}
		
		public function get position():Point 
		{
			return _position;
		}
		
		public function set position(value:Point):void 
		{
			_position = value;
		}
		
		public function get graphics():Graphics 
		{
			return _graphics;
		}
		
		public function set graphics(value:Graphics):void 
		{
			_graphics = value;
		}
		
		
		public function get speed():Number 
		{
			return _speed;
		}
		
		public function set speed(value:Number):void 
		{
			_speed = value;
		}
		
		public function set isMoved(value:Boolean):void 
		{
			_isMoved = value;
		}
		
		public function get isMoved():Boolean 
		{
			return _isMoved;
		}
		
		public function get moveVector():Point 
		{
			return _moveVector;
		}
		
		public function set moveVector(value:Point):void 
		{
			_moveVector = value;
		}
		
		public function isPlaced():Boolean 
		{
			return _position.x >= 0 && _position.y >= 0;
		}
		
		public function draw():void
		{
			graphics.draw(this);
		}

		public function move(dt:Number):void {
			if(_isMoved){
				position = setNewPositionDt(dt);
				if (speed >= Game.resistance * dt) {
					speed -= Game.resistance * dt;
				}else {
					speed = 0;
					_isMoved = false;
				}
			}
		}		
		
		private function isHitLine(beginLine : Point, endLine : Point, dt : Number) : Boolean {
			//вычисляем следующую позицию
			var newPosition : Point = setNewPositionDt(dt);	
			//проверяем расстояние от нее до границы
			var d : Number = Utils.getDistanseFromPointToLine(newPosition, beginLine, endLine);
			//считаем текущее расстояние до границы 
			var nowDist:Number = Utils.getDistanseFromPointToLine(position, beginLine, endLine);
			//не перелетел ли наш шар за границу
			var pointsAtSamePlain:Boolean = Utils.isPointsAtSamePlain(position, newPosition, beginLine, endLine);
			//шар касается границы
			if (Math.ceil(d) == radius && pointsAtSamePlain) {
				return true;
			}
			//шар не долетел до границы
			if (Math.ceil(d) > radius && pointsAtSamePlain) {
				return false;
			}	
			//вычисляем расстояние, которое пролетел шар за момент времени
			var distByDt:Number = pointsAtSamePlain ? (nowDist - d) : (nowDist + d);
			//сколько должен был пролететь шар
			var needDist:Number = nowDist - radius;
			//вычисляем время, за которое шар пройдет необходимое расстояние
			var newDt:Number = (needDist * dt) / distByDt;
			//передвигаем шар на это время
			move(newDt);
			draw();
			
			return true;
		}		
		
		private function setNewPositionDt(dt:Number):Point 
		{
			var x : Number = position.x + _moveVector.x * dt * speed;
			var y : Number = position.y + _moveVector.y * dt * speed;
			return new Point(x, y);
		}				
		
		//отражение от границы
		public function reflection(beginLine : Point, endLine : Point) : void {
			var normal : Point = Utils.getNormal(beginLine, endLine);

			var dot:Number = Utils.getScalar(_moveVector, normal);
			_moveVector.x -= 2 * dot * normal.x;
			_moveVector.y -= 2 * dot * normal.y
		}	
		
		public function checkHitBorders(borders:Vector.<Border>, dt:Number):void 
		{
			for (var i : int = 0; i < borders.length; i++ ) {
				if (isHitLine(borders[i].begin, borders[i].end, dt) == true) {						
					reflection(borders[i].begin, borders[i].end);					
					
					SoundController.playSound(new showLine1, 0.7);
					
					return;
				}
			}
		}
				
		public function getSquare():Number 
		{
			return Math.round(Math.PI * radius * radius);
		}

	}
}