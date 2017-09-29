package logic
{
	import flash.geom.Point;
	
	/**
	 * ...
	 * @author Marshalova Valeriya
	 */
	public class TetrisShape 
	{		
		private var _type:int; // shape index in Config.SHAPES 
		private var _arrSquares:Array /* of Points */; // array of coordinates each point in shape
		private var _position:Point;
		
		public function TetrisShape():void 
		{	
			createShape();
			_position = new Point(Config.DEFAULT_SHAPE_POSITION_H, Config.DEFAULT_SHAPE_POSITION_W);						
		}
		
		/**
		 * create random shape with random rotation
		 */
		private function createShape():void 
		{			
			_arrSquares = new Array();
			_type = Math.random() * 100 % (Config.SHAPES.length - 1) + 1;	
			_arrSquares = Config.SHAPES[_type].points;			
			var cntRotate:int = Math.random() * 100 % 4;
			while (cntRotate--) 
			{
				rotate();
			}
		}
		
		/**
		 * change shape position
		 * @param	$direction
		 */
		public function move($direction:int):void 
		{
			switch($direction) 
			{
				case Config.DOWN:
				case Config.QUICK_DOWN:
					position.x ++;
				break;
				case Config.LEFT:
					position.y --;
				break;
				case Config.RIGHT:
					position.y ++;
				break;
				case Config.ROTATE:
					rotate();
			}
		}
		
		/**
		 * calculate points which must be checked for shape move
		 * @param	$direction
		 * @return array of glass array points according to direction which must be checked
		 */
		public function checkPoints($direction:int):Array /*of Points*/
		{
			var check:Array = new Array();
			var item:Point;
			var newPoint:Point;
			switch($direction) 
			{				
				case Config.DOWN: 
				case Config.QUICK_DOWN:
					for each(item in _arrSquares) 
					{
						newPoint= new Point(position.x + item.x + 1, position.y + item.y);							
						if (!check[item.y]) check[item.y] = newPoint;
						else if (newPoint.x > check[item.y].x) check[item.y] = newPoint;
					}
				break;
				case Config.LEFT:
					for each(item in _arrSquares) 
					{
						newPoint = new Point(position.x + item.x, position.y + item.y - 1);						
						if (!check[item.x]) check[item.x] = newPoint;
						else if (newPoint.y < check[item.x].y) check[item.x] = newPoint;
					}
				break;
				case Config.RIGHT:
					for each(item in _arrSquares) 
					{
						newPoint = new Point(position.x + item.x, position.y + item.y + 1);						
						if (!check[item.x]) check[item.x] = newPoint;
						else if (newPoint.y > check[item.x].y) check[item.x] = newPoint;
					}
				break;
				case Config.ROTATE:
					check = getRotatedShape(true);									
				break;		
				case Config.BEGIN_POINTS:
					for each(item in _arrSquares) 
					{
						newPoint = new Point(position.x + item.x, position.y + item.y);							
						check.push(newPoint);						
					}
				break;
			}
			return check;
		}			
		
		/**
		 * get rotated shape array
		 * @param	$withPosition (if true returns shape array points inside glass, otherwise returns points inside shape)
		 * @return
		 */
		private function getRotatedShape($withPosition:Boolean = false):Array /*of Points*/
		{
			var rotatedShape:Array = new Array();
			
			var n:int = maxHeight;
			var m:int = maxWidth;
			
			var oldArr:Array = [];
			for (var i:int = 0; i < n; i++) 
			{
				oldArr[i] = [];
				for (var j:int = 0; j < m; j++) 
				{
					oldArr[i][j] = 0;
				}
			}	
			for each(var p:Point in _arrSquares) 
			{												
				oldArr[p.x][p.y] = 1;
				
			}			
			for(i = 0;i<m;i++)
			{
				for(j=0;j<n;j++)
				{
					var isEmpty:Boolean = oldArr[j][m - 1 - i] == 0;					
					if (!isEmpty) {
						rotatedShape.push($withPosition ? new Point(position.x + i, position.y + j) : new Point(i, j));
					}
				}
			}				
			return rotatedShape;
		}
		
		/**
		 * rotate shape
		 */
		public function rotate():void
		{	
			_arrSquares = getRotatedShape();
		}		
		
		
		//--------------------------------------------------------------------------
		//
		//  Getters&setters
		//
		//--------------------------------------------------------------------------
		
		private function get maxWidth():int 
		{
			var max:int = 0;
			for each(var p:Point in _arrSquares) 
			{
				if (p.y > max) max = p.y;
			}			
			return max + 1;
		}
		
		private function get maxHeight():int 
		{
			var max:int = 0;
			for each(var p:Point in _arrSquares) 
			{
				if (p.x > max) max = p.x;
			}			
			return max + 1;
		}		
		
		public function get position():Point 
		{
			return _position;
		}	
		
		public function get arrSquares():Array 
		{
			return _arrSquares;
		}
		
		public function get type():int 
		{
			return _type;
		}		
	}
	
}