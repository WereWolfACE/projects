package  
{
	import flash.display.Sprite;
	import flash.geom.Point;
	
	/**
	 * ...
	 * @author Marshalova Valery
	 */
	public class Border extends Sprite 
	{
		private var _begin : Point;
		private var _end : Point;
		
		public function Border(begin:Point, end:Point) 
		{
			_begin = begin;
			_end = end;
			
			graphics.lineStyle(2, 0xFF0000);
			graphics.moveTo(_begin.x, _begin.y);
			graphics.lineTo(_end.x, _end.y);		
			
		}
		
		public function get begin():Point 
		{
			return _begin;
		}
		
		public function set begin(value:Point):void 
		{
			_begin = value;
		}
		
		public function get end():Point 
		{
			return _end;
		}
		
		public function set end(value:Point):void 
		{
			_end = value;
		}
		
	}

}