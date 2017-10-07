package  
{
	import flash.display.Sprite;
	import flash.events.EventDispatcher;
	/**
	 * ...
	 * @author Marshalova Valery
	 */
	public class CollisionGrid extends EventDispatcher
	{
		private const EMPTY:int = 0;
		private const CIRCLES:int = 1;
		
		private var _cellSize : int;
		private var _circles : Vector.<BaseCircle>;
		private var _grid : Array;		
		private var _emptyGrid:Array;
		
		public function CollisionGrid() 
		{

		}
		
		public function refresh():void {
			if(_circles){
				calculateCellSize();				
			}
		}
		
		public function startChecking():void {
			if (_circles) {
				create();
				assignCirclesToGrid();
				check();
			}
		}
		
		//просчет размера ячейки грида (должна быть больше самого большого шара)
		public function calculateCellSize():void {
			var maxSize:Number = 0;
			for each (var circle:BaseCircle in _circles) {
				maxSize = maxSize > circle.radius ? maxSize : circle.radius;
			}
			maxSize = maxSize * 2 + 2;
			_cellSize = maxSize > 100 ? maxSize : 100;
		}
		
		public function create():void {
			_grid = new Array();
			_emptyGrid = new Array();
			for(var i:int = 0; i < Game.fieldWidth / _cellSize; i++){
				_grid[i] = new Array();	
				_emptyGrid[i] = new Array();
				for(var j:int = 0; j < Game.fieldHeight / _cellSize; j++){
					_grid[i][j] = new Array();
					_emptyGrid[i][j] = 0;
				}
			}			
		}
		
		//привязка шаров, находящихся в области ячейки, к каждой ячейке грида
		private function assignCirclesToGrid():void
		{
			for(var i:int = 0; i < _circles.length; i++){
				var circle:BaseCircle = _circles[i] as BaseCircle;
				var xpos:int = Math.floor(circle.position.x / _cellSize);
				var ypos:int = Math.floor(circle.position.y / _cellSize);
				_grid[xpos][ypos].push(circle);
				_emptyGrid[xpos][ypos] += 1;
			}
		}
 
		//проверка на столкновение шаров в текущей ячейки и со всеми прилежащими
		private function check():void
		{
			for(var i:int = 0; i < _grid.length; i++){
				for(var j:int = 0; j < _grid[i].length; j++){
					if(checkCurrentCell(i, j) == EMPTY) continue;
					checkNeighboringCell(i, j, i + 1, j);     // проверка с объектами ячейки справа
					checkNeighboringCell(i, j, i - 1, j + 1); // проверка с объектами ячейки снизу слева
					checkNeighboringCell(i, j, i, j + 1);     // проверка с объектами ячейки снизу
					checkNeighboringCell(i, j, i + 1, j + 1); // проверка с объектами ячейки снизу справа
				}
			}
		}
 
		private function checkCurrentCell(x:int, y:int):int
		{
			if (_emptyGrid[x][y] == EMPTY) return EMPTY;
			if (_emptyGrid[x][y] == 1) return CIRCLES;
			
			var cell:Array = _grid[x][y] as Array;
			
			for(var i:int = 0; i < cell.length - 1; i++){
				var circle1:BaseCircle = cell[i] as BaseCircle;
				for(var j:int = i + 1; j < cell.length; j++){
					var circle2:BaseCircle = cell[j] as BaseCircle;
					checkCollision(circle1, circle2);
				}
			}
			return CIRCLES;
		}
 
		private function checkNeighboringCell(x1:int, y1:int, x2:int, y2:int):void 
		{
			if(x2 < 0 || x2 >= _grid.length || y2 >= _grid[x2].length) return;
 	
			if (_emptyGrid[x1][y1] == EMPTY || _emptyGrid[x2][y2] == EMPTY) return;
			
			var cell0:Array = _grid[x1][y1] as Array;
			var cell1:Array = _grid[x2][y2] as Array;			
			
			for(var i:int = 0; i < cell0.length; i++){
				var circle1:BaseCircle = cell0[i] as BaseCircle;
				for(var j:int = 0; j < cell1.length; j++){
					var circle2:BaseCircle = cell1[j] as BaseCircle;
					checkCollision(circle1, circle2);
				}
			}
		}
 
		private function checkCollision(circle1:BaseCircle, circle2:BaseCircle):void
		{
			var dist:Number = Utils.getDistanseFromPointToPoint(circle1.position, circle2.position);
			if (dist <= circle1.radius + circle2.radius) {
				var minCircle:BaseCircle;
				var maxCircle:BaseCircle;
				if (circle1.radius == circle2.radius) {
					var fortuna:int = Math.round(Math.random());
					if (fortuna == 0) {
						minCircle = circle1;
						maxCircle = circle2;
					}else {
						minCircle = circle2;
						maxCircle = circle1;
					}
				}else{
					minCircle = circle1.radius > circle2.radius ? circle2 : circle1;
					maxCircle = circle1.radius > circle2.radius ? circle1 : circle2;
				}
				
				dispatchEvent(new ApplicationEvents(ApplicationEvents.HIT_CIRCLES, 
													{maxCircle:maxCircle, minCircle:minCircle}));
			}
		}
		
		public function get cellSize():int 
		{
			return _cellSize;
		}
		
		public function set cellSize(value:int):void 
		{
			_cellSize = value;
		}
		
		public function get circles():Vector.<BaseCircle> 
		{
			return _circles;
		}
		
		public function set circles(value:Vector.<BaseCircle>):void 
		{
			_circles = value;
		}
		
	}

}