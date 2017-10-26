package logic
{
	import flash.display.Sprite;
	import flash.events.KeyboardEvent;
	import flash.events.TimerEvent;
	import flash.geom.Point;
	import flash.utils.Timer;
	
	/**
	 * ...
	 * @author Marshalova Valeriya
	 */
	public class GameField 
	{		
		private var _arrGlass : Array /*of Numbers*/;
		private var _currentShape : TetrisShape;
		private var _nextShape : TetrisShape;
		private var _height:int;
		private var _width:int;
		private var _timer:Timer;		
		
		public function GameField($width:int, $height:int) 
		{
			_width = $width;
			_height = $height;			
			_arrGlass = [];
			for (var i:int = 0; i < height; i++) 
			{
				_arrGlass[i] = [];
				for (var j:int = 0; j < width; j++) 
				{
					_arrGlass[i][j] = 0;
				}
			}				
			
			_timer = new Timer(Config.DEFAULT_TIME);
			_timer.addEventListener(TimerEvent.TIMER, onTimer);
			Main.getInstance().stage.addEventListener(KeyboardEvent.KEY_DOWN, onKeyDown);
		}
		
		/**
		 * move shape on game timer
		 * @param	$e
		 */
		private function onTimer($e:TimerEvent):void 
		{
			moveShape(Config.DOWN);
		}
		
		private function onKeyDown($e:KeyboardEvent):void 
		{			
			if ($e.keyCode == 27)    // ESC
			{
				if (isEndGame)
				{	
					Main.getInstance().stage.removeEventListener(KeyboardEvent.KEY_DOWN, onKeyDown);
					Game.getInstance().destroy();
					Game.getInstance().run();
					return;
				}
				isPause ? _timer.start() : _timer.stop();				
				drawPause(isPause);
				return;
			}	
			if (isPause) return;
			var direction:int = -1;
			switch ($e.keyCode) 
			{
				case 32:	// space			
					direction = Config.QUICK_DOWN;
				break;
				case 37:	// left			
					direction = Config.LEFT;
				break;
				case 38:    // up
					direction = Config.ROTATE;
				break;
				case 39:    // right
					direction = Config.RIGHT;
				break;
				case 40:    // down
					direction = Config.DOWN;
				break;				
			}
			if (direction > -1)
				moveShape(direction);
		}	
		
		/**
		 * moved shape
		 * @param	$direction
		 */		
		private function moveShape($direction:int):void 
		{			
			switch($direction) 
			{
				case Config.QUICK_DOWN:
					moveShapeQuick($direction);
				break;
				default:
					moveShapeStep($direction);
			}
		}	
		
		/**
		 * move shape by one step
		 * @param	$direction
		 */
		private function moveShapeStep($direction:int):void 
		{
			if (isMoveAvailable($direction)) 
			{
				_currentShape.move($direction);
				drawShape();
			}
			else 
			{
				if ($direction == Config.DOWN) 
				{					
					addShape();
					_timer.stop();
					play();
				}
			}
		}		
		
		/**
		 * move shape while it possible
		 * @param	$direction
		 */
		private function moveShapeQuick($direction:int):void 
		{
			_timer.stop();
			while (isMoveAvailable($direction)) 
			{
				_currentShape.move($direction);
				drawShape();
			}
			addShape();			
			play();		
		}
		
		/**
		 * add shape to glass array
		 */
		private function addShape():void 
		{
			for each(var point:Point in _currentShape.arrSquares) 
			{
				_arrGlass[_currentShape.position.x + point.x][_currentShape.position.y + point.y] = _currentShape.type;
			}
		}	
		
		/**
		 * check shape's possibility to move in the direction
		 * @param	$direction
		 * @return  check result (true if moved shape's points are empty in glass array, otherwise false) 
		 */
		private function isMoveAvailable($direction:int):Boolean 
		{
			var points:Array = _currentShape.checkPoints($direction);			
			for each (var point:Point in points) 
			{				
				if (point.y < 0 || point.x >= height || point.y >= width) 
					return false;
				if (_arrGlass[point.x][point.y] != Config.EMPTY) 
					return false;				
			}			
			return true;
		}	
		
		/**
		 * start new game round
		 */
		public function play():void 
		{			
			if (_currentShape)				
				Game.getInstance().updateUserExp(getCollectedRows());				
				
			if(!_nextShape)
				_currentShape = new TetrisShape();		 		
			else 
				_currentShape = _nextShape;
						
			_nextShape = new TetrisShape();	
			
			drawNextShape();
			drawGlass();			
			drawShape();			
			
			if (!isEndGame)
			{
				_timer.delay = Game.getInstance().getShapeFallingTime();
				_timer.start();				
			}
			else
			{
				finishGame();
			}
		}
		
		/**
		 * playing stop
		 */
		private function finishGame():void 
		{			
			Game.getInstance().finishGame(true);			
		}	
		
		/**
		 * check and delete collected rows from glass array
		 * @return collected rows count
		 */
		private function getCollectedRows():int
		{
			var collectedRows:int = 0;
			var newArr:Array = [];
			for (var i:int = 0; i < height; i++ ) 
			{				
				if (!isCollectedRow(_arrGlass[i])) 
					newArr.push(_arrGlass[i]);
				else 
					collectedRows++;				
			}			
			if (collectedRows > 0)
			{
				var arr:Array = [];
				// add new rows 
				for (var j:int = 0; j < width; j++) 
				{
					arr.push(0);
				}
				while (newArr.length < height)
				{
					newArr.unshift(arr);
				}				
				_arrGlass = newArr;				
			}
			return collectedRows;						
		}
		
		/**
		 * check row is collect
		 * @param	$lineArray
		 * @return  true if each row's element is not empty, otherwice false
		 */
		private function isCollectedRow($row:Array /* of Numbers */):Boolean 
		{			
			for (var i:int = 0; i < $row.length; i++ ) 
			{
				if ($row[i] == Config.EMPTY) 				
					return false;
			}			
			return true;
		}
		
		//--------------------------------------------------------------------------
		//
		//  Drawing methods
		//
		//--------------------------------------------------------------------------
		
		private function drawGlass():void 
		{				
			Game.getInstance().drawField(_arrGlass);				
		}
		
		private function drawShape():void 
		{				
			Game.getInstance().drawShape(_currentShape);									
		}
		
		private function drawNextShape():void 
		{
			Game.getInstance().drawNextShape(_nextShape);		
		}

		private function drawPause($isPause:Boolean):void 
		{
			Game.getInstance().drawPause($isPause);		
		}
		
		
		//--------------------------------------------------------------------------
		//
		//  Getters&setters
		//
		//--------------------------------------------------------------------------

		public function get height():int 
		{
			return _height;
		}
		
		public function get width():int 
		{
			return _width;
		}		
		
		private function get isEndGame():Boolean 
		{
			if(!isMoveAvailable(Config.BEGIN_POINTS))
				return true;
			return false;
		}	
		
		private function get isPause():Boolean 
		{
			if(_timer.running)
				return false;
			return true;
		}
	}
	
}