package  
{
	import Constants.GameConstants;
	import Constants.GameEvents;
	import Constants.GameObjectsTypes;
	import flash.geom.Rectangle;
	import GameElements.GameObject;
	/**
	 * ...
	 * @author Marshalova Valery
	 */
	public class CollisionGrid
	{
		private const EMPTY:int = 0;
		private const OBJECTS:int = 1;
		
		private var _cellSize : int;
		private var _objects : Vector.<GameObject>;
		private var _grid : Array;		
		private var _emptyGrid:Array;
		
		public function CollisionGrid() 
		{
			calculateCellSize();
		}
		
		public function refresh(evilBullets:Vector.<GameObject>, heroBullets:Vector.<GameObject>, evilGuys:Vector.<GameObject>, hero:GameObject):void {
			_objects = null;
			var v:Vector.<GameObject> = new Vector.<GameObject>();
			v.push(hero);
			_objects = evilBullets.concat(heroBullets, evilGuys, v);
		}
		
		public function startChecking():void {
			if (_objects) {
				create();
				assignObjectsToGrid();
				check();
			}
		}
		
		//просчет размера ячейки грида (должна быть больше самого большого объекта)
		public function calculateCellSize():void {
			_cellSize = GameConstants.CHARACTER_WIDTH;
		}
		
		public function create():void {
			_grid = new Array();
			_emptyGrid = new Array();
			var colsCount:int = StarlingMain.stage.stageWidth / _cellSize;
			var rowsCount:int = StarlingMain.stage.stageHeight / _cellSize;
			
			for(var i:int = 0; i < colsCount; i++){
				_grid[i] = new Array();	
				_emptyGrid[i] = new Array();
				for(var j:int = 0; j < rowsCount; j++){
					_grid[i][j] = new Array();
					_emptyGrid[i][j] = EMPTY;
				}
			}			
		}
		
		//привязка объектов, находящихся в области ячейки, к каждой ячейке грида
		private function assignObjectsToGrid():void
		{
			for(var i:int = 0; i < _objects.length; i++){
				var object:GameObject = _objects[i] as GameObject;
				var xpos:int = Math.floor(object.x / _cellSize);
				var ypos:int = Math.floor(object.y / _cellSize);
				if (ypos <= 0 || ypos > StarlingMain.stage.stageHeight) continue;
				if (xpos <= 0 || xpos > StarlingMain.stage.stageWidth) continue;
				if (!_grid[xpos] || !_grid[xpos][ypos]) continue;
				_grid[xpos][ypos].push(object);
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
			if (_emptyGrid[x][y] == 1) return OBJECTS;
			
			var cell:Array = _grid[x][y] as Array;
			
			for(var i:int = 0; i < cell.length - 1; i++){
				var object1:GameObject = cell[i] as GameObject;
				for(var j:int = i + 1; j < cell.length; j++){
					var object2:GameObject = cell[j] as GameObject;
					checkTypesForCollision(object1, object2);
				}
			}
			return OBJECTS;
		}
		
		private function checkTypesForCollision(object1:GameObject, object2:GameObject):void 
		{
			//если это вражеская пуля или враг, смотрим, могут ли они столкнуться с героем
			var yForHeroCollision:int = StarlingMain.stage.stageHeight - GameConstants.CHARACTER_HEIGHT;
			
			if (object1.typeToCollision == GameObjectsTypes.EVIL_BULLET) { //вражеская пуля с героем
				if (object1.realPosition.y < yForHeroCollision) {
					return;
				}else {
					if(object2.typeToCollision == GameObjectsTypes.HERO){
						checkCollision(object1, object2);
					}else {
						return;
					}
				}
			}else if (object2.typeToCollision == GameObjectsTypes.EVIL_BULLET) { //герой с вражеской пулей
				if (object2.realPosition.y < yForHeroCollision) {
					return;
				}else {
					if(object1.typeToCollision == GameObjectsTypes.HERO){
						checkCollision(object1, object2);
					}else {
						return;
					}
				}
			}else if (object1.typeToCollision == GameObjectsTypes.EVIL_GUY) { // враг с героем
				if(object2.typeToCollision == GameObjectsTypes.HERO){
					if (object1.realPosition.y < yForHeroCollision) {
						return;
					}else {
						checkCollision(object1, object2);
					}
				}else if (object2.typeToCollision == GameObjectsTypes.HERO_BULLET) { //враг с пулей героя
						checkCollision(object1, object2);
				}else {
					return;
				}					
			}else if (object2.typeToCollision == GameObjectsTypes.EVIL_GUY) { //герой с врагом
				if(object1.typeToCollision == GameObjectsTypes.HERO){
					if (object2.realPosition.y < yForHeroCollision) {
						return;
					}else {						
						checkCollision(object1, object2);
					}
				}else if(object1.typeToCollision == GameObjectsTypes.HERO_BULLET) { //пуля героя с врагом					
						checkCollision(object1, object2);
				}else {
					return;
				}	
				
			 
			}
		}
 
		private function checkNeighboringCell(x1:int, y1:int, x2:int, y2:int):void 
		{
			if(x2 < 0 || x2 >= _grid.length || y2 >= _grid[x2].length) return;
 	
			if (_emptyGrid[x1][y1] == EMPTY || _emptyGrid[x2][y2] == EMPTY) return;
			
			var cell0:Array = _grid[x1][y1] as Array;
			var cell1:Array = _grid[x2][y2] as Array;			
			
			for(var i:int = 0; i < cell0.length; i++){
				var object1:GameObject = cell0[i] as GameObject;
				for(var j:int = 0; j < cell1.length; j++){
					var object2:GameObject = cell1[j] as GameObject;
					checkTypesForCollision(object1, object2);
				}
			}
		}
 
		private function checkCollision(object1:GameObject, object2:GameObject):void
		{
			var bounds1:Rectangle = object1.bounds;
			var bounds2:Rectangle = object2.bounds;
			
			if (bounds1.intersects(bounds2))
			{
				StarlingGlobalEventDispatcher.instance.dispatchEventWith(GameEvents.COLLISION, false, { object1:object1, object2:object2 } );
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
		
		public function get objects():Vector.<GameObject> 
		{
			return _objects;
		}
		
		public function set objects(value:Vector.<GameObject>):void 
		{
			_objects = value;
		}

	}

}