package  
{
	import flash.display.Sprite;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.events.TimerEvent;
	import flash.geom.Point;
	import flash.geom.Rectangle;
	import flash.text.TextField;
	import flash.text.TextFieldAutoSize;
	import flash.text.TextFormat;
	import flash.utils.Timer;
	
	/**
	 * ...
	 * @author Marshalova Valery
	 */
	public class Game extends Sprite 
	{
		static public const ENEMIES_WIN_BY_SQUARE:int = -1;
		static public const PLAYER_WIN_BY_SQUARE:int = 1;
		static public const NO_WIN_BY_SQUARE:int = 0;
		static public const PLAYER_LOOSE:int = -2;
				
		private var _gameEnd:Boolean;
		
		//массив всех шаров
		private var _circles : Vector.<BaseCircle>;
		//массив прямоугольников для расстановки шаров
		private var _rectangles : Vector.<Rectangle>;
		//массив для проверки столкновений
		private var _grid : CollisionGrid;
		//шар игрока
		private var _player : PlayerCircle;
		//границы игрового поля
		private var _borders:Vector.<Border>;
		
		private var _rootContainer:Sprite;
		private var _textContainer:Sprite;
		
		private var _beginTime : int;
		private var _endTime : int;		
		private var _timer:Timer;
		
		private var _count:int;
		private var _minRadius : int;
		private var _maxRadius : int;
		private var _userColor : int;
		private var _userRadius : int;
		private var _enemyColor1 : Array;
		private var _enemyColor2 : Array;
		
		private static var _fieldWidth : int;
		private static var _fieldHeight: int;
		//сопротивление пространства
		private static var _resistance:int;
		
		public function Game(count : int, minRadius : int, maxRadius : int, userRadius : int,
							userColor : int, enemyColor1 : Array, enemyColor2 : Array, 
							fieldWidth : int, fieldHeight: int, resistance:int) {
				
			_count = count;
			_minRadius = minRadius;
			_maxRadius = maxRadius;
			_userColor = userColor;
			_userRadius = userRadius;
			_enemyColor1 = enemyColor1;
			_enemyColor2 = enemyColor2;
			
			Game._fieldWidth = fieldWidth;
			Game._fieldHeight = fieldHeight;			
			Game._resistance = resistance;		
			
			SoundController.playSound(new bunusGameBackground, 0.5, 1000);			
		}
		
		static public function get fieldWidth():int 
		{
			return _fieldWidth;
		}
		
		static public function get fieldHeight():int 
		{
			return _fieldHeight;
		}
		
		static public function get resistance():int 
		{
			return _resistance;
		}
		
		// создание и отрисовка границ игрового поля
		private function drawField():void 
		{
			_borders = new Vector.<Border>;
			_borders.push(new Border(new Point(0, 0), new Point(_fieldWidth, 0)));
			_borders.push(new Border(new Point(0, 0), new Point(0, _fieldHeight)));
			_borders.push(new Border(new Point(_fieldWidth, 0), new Point(_fieldWidth, _fieldHeight)));
			_borders.push(new Border(new Point(0, _fieldHeight), new Point(_fieldWidth, _fieldHeight)));
			
			for (var i : int = 0; i < _borders.length; i++ ) {
				_rootContainer.addChild(_borders[i]);
			}
			
			_rootContainer.graphics.beginFill(0xcccccc, 1);
			_rootContainer.graphics.drawRect(0, 0, _fieldWidth, _fieldHeight);
			_rootContainer.graphics.endFill();
		}
		
		private function onTimer(e:TimerEvent):void 
		{
			var dt:Number = _timer.delay / 1000;
			
			for (var i:int = 0; i < _circles.length; i++) {
				_circles[i].checkHitBorders(_borders, dt);
				_circles[i].move(dt);
			}			
		}		
			
		private function onMouseUp(e:MouseEvent):void 
		{
			_endTime = new Date().getTime();
			var duration : int = _endTime - _beginTime;
			var vector : Point = Utils.getVector(new Point(e.stageX, e.stageY), _player.position);
			_player.setMoveVector(vector, duration);
			_player.isMoved = true;
		}
		
		private function onMouseDown(e:MouseEvent):void 
		{
			_beginTime = new Date().getTime();
		}
		
		private function onEnterFrame(e:Event):void 
		{
			_grid.startChecking();
			if (_circles) {
				drawCircles();
			}
		}			
		
		private function drawCircles():void 
		{
			for (var i : int = 0; i < _circles.length; i++ ) {
				_circles[i].draw();
				_rootContainer.addChild(_circles[i].graphics);
			}
		}
		
		//создание и расстановка врагов
		private function createAndPlacingCircles():void 
		{			
			var smallestRadius:int = _maxRadius;
			var biggestRadius:int = _minRadius;
			for (var i:int = 0; i < _count; i++) {
				var r: int = Math.floor(Math.random() * (_maxRadius - _minRadius) + _minRadius);
				var enemy:EnemyCircle = new EnemyCircle(_enemyColor1, _enemyColor2, _player.radius);
				enemy.radius = r;
				var vector: Point = Utils.getVector(new Point(Math.random() * _fieldWidth, Math.random() * _fieldHeight), enemy.position);
				enemy.setMoveVector(vector);
				//максимальная скорость врага равна его диаметру, чтобы не получалось ситуаций,
				//когда четырехпиксельный круг имеет скорость в 1000 пикселей, и не двигается, 
				//а перескакивает с места на место 
				var maxSpeed:Number = 2 * r;
				maxSpeed = (maxSpeed > BaseCircle.MAX_SPEED) ? BaseCircle.MAX_SPEED : maxSpeed; 
				enemy.speed = Math.random() * (maxSpeed - BaseCircle.MIN_SPEED) + BaseCircle.MIN_SPEED;
				_circles.push(enemy);
				
				smallestRadius = (r < smallestRadius) ? r : smallestRadius;
				biggestRadius = (r > biggestRadius) ? r : biggestRadius;
			}
					
			_circles.push(_player);
			_circles.sort(Utils.sortCircles);	
			
			//размер ячейки грида для расстановки равен размеру самого маленького шарика 
			var gridWidth:int = 2 * smallestRadius;
			var gridHeight:int = gridWidth;
			
			var delta:int = biggestRadius;
			var colsCount:int = Math.floor((_fieldWidth - delta * 2) / gridWidth);
			var rowsCount:int = Math.floor((_fieldHeight - delta * 2) / gridHeight);
			
			_rectangles = new Vector.<Rectangle>;
			
			for (i = 0; i < colsCount; i++) {
				for (var j : int = 0; j < rowsCount; j++) {
					_rectangles.push(new Rectangle(
										delta + i * gridWidth,
										delta + j * gridHeight,
										gridWidth,
										gridHeight									
									));					
				}
			}
						
			var isPlaceable:Boolean = true;
			for (i = 0; i < _circles.length; i++) {
				if (_rectangles.length > 0) {					
					var circle:BaseCircle = _circles[i];
					if (circle.isPlaced()) continue;
					isPlaceable = true;
					var index:int = Math.floor(Math.random() * (_rectangles.length - 1));				
					var xCoord:Number = _rectangles[index].x + _rectangles[index].width / 2;
					var yCoord:Number = _rectangles[index].y + _rectangles[index].height / 2;

					for (j = 0; j < i; j++) {
						var placedCircle:BaseCircle = _circles[j];
						if (!placedCircle.isPlaced()) continue;
						var d:int = Utils.getDistanseFromPointToPoint(placedCircle.position, new Point(xCoord, yCoord));
						if (d <= placedCircle.radius + circle.radius) {		
							isPlaceable = false;
							break;
						}
					}
					if (isPlaceable) {
						circle.position.x = xCoord;
						circle.position.y = yCoord;
					}else {
						//если текущий шар не подходит по размерам в выбранную ячейку, 
						//берем шар меньшего диаметра						
						for (var k:int = i + 1; k < _circles.length; k++) {
							var smallestCircle: BaseCircle = _circles[k];
							if (!smallestCircle.isPlaced()) continue;
							for (j = 0; j < i; j++) {
								var placedCircle:BaseCircle = _circles[j];
								if (!placedCircle.isPlaced()) continue;
								var d:int = Utils.getDistanseFromPointToPoint(placedCircle.position, new Point(xCoord, yCoord));
								if (d > placedCircle.radius + circle.radius) {		
									isPlaceable = true;
									smallestCircle.position.x = xCoord;
									smallestCircle.position.y = yCoord;
									_circles[i] = smallestCircle;
									_circles.splice(k, 1);
									_circles.splice(i + 1, 0, circle);
									break;
								}
							}							
						}
						//если в выбранную ячейку не поставили никакого шара, 
						//то остаемся на том же элементе массива
						if (!isPlaceable) {
							i--;
						}
					}
					//выбранная ячейка больше не будет использоваться в расстановке
					_rectangles.splice(index, 1);
				}else {
					//если шаров намного больше, чем свободного места, удалем лишние шары
					_circles.splice(i, _circles.length - i);
					break;
				}
			}
			_grid.circles = _circles;
			_grid.refresh();
			for (i = 0; i < _circles.length; i++ ) {
				_rootContainer.addChild(_circles[i].graphics);
				if (_circles[i] != _player) _circles[i].isMoved = true;
			}
		}
						
		private function onHitCircles(e:ApplicationEvents):void {
			var maxCircle:BaseCircle = e.params.maxCircle;
			var minCircle:BaseCircle = e.params.minCircle;
			var square:Number = Utils.intersectionSquare(maxCircle, minCircle);			
			_rootContainer.removeChild(minCircle.graphics);
			_rootContainer.addChildAt(minCircle.graphics, _rootContainer.getChildIndex(maxCircle.graphics));
			swallow(minCircle, maxCircle, square);			
			_grid.refresh();
			if (maxCircle is PlayerCircle) {
				for (var i:int = 0; i < _circles.length; i++) {
					if(_circles[i] is EnemyCircle) (_circles[i] as EnemyCircle).playerRadius = maxCircle.radius;
				}
			}
		}
		
		//функция поглощения одного шарика другим 
		public function swallow(circle1 : BaseCircle, circle2:BaseCircle, square:Number) : void {			
			var circle1Square:Number = circle1.getSquare();
			var circle2Square:Number = circle2.getSquare();
			var resultSquare1:Number = circle1Square - square;
			var resultSquare2:Number = circle2Square + square;
			var circle1radius:Number = Math.sqrt(resultSquare1 / Math.PI);
			var circle2radius:Number = Math.sqrt(resultSquare2 / Math.PI);
			
			if (isNaN(circle1radius) || circle1radius <= 0)
			{
				_rootContainer.removeChild(circle1.graphics);
				_circles.splice(_circles.indexOf(circle1), 1);				
				
				if (!_gameEnd) {					
					if (circle1 is PlayerCircle) { //игрока съели
						SoundController.playSound(new destroyLine, 0.5);
						showGameEndText(Game.PLAYER_LOOSE);
						_gameEnd = true;
					}else {
						var win:int = winBySquare();
						if (win != Game.NO_WIN_BY_SQUARE) {
							SoundController.playSound(new spinComplete);
							showGameEndText(win);
							_gameEnd = true;
						}
					}
				}
				return;
			}
			circle1.radius = circle1radius;
			circle2.radius = circle2radius;
		}
		
		public function showGameEndText(win:int):void 
		{				
			var result:TextField = new TextField();            
			var text:String = "";
			switch(win) {
				case Game.PLAYER_LOOSE:
					text = "Поражение :( Вашим героем пообедали";
					break;
				case Game.ENEMIES_WIN_BY_SQUARE:
					text = "Поражение :( Здесь больше нет врагов, которые Вашему герою по зубам";
					break;
				case Game.PLAYER_WIN_BY_SQUARE:
					text = "Победа! Вы круче всех!!!";
					break;
			}
            result.textColor = 0x0;		
			
            var format1:TextFormat = new TextFormat("Arial", 36, 0x0, true);
			result.defaultTextFormat = format1;
			result.autoSize = TextFieldAutoSize.CENTER;
			result.selectable = false;
			result.text = text + "\nНачать новую игру - Ctrl + N";
			result.x = _fieldWidth / 2 - result.width / 2;
			result.y = _fieldHeight / 2 - result.height / 2;
            _textContainer.addChild(result);
		}

		private function winBySquare():int {
			var s:Number = _player.getSquare();
			_circles.sort(Utils.sortCircles);
			//игрок больше не может никого съесть
			if (_circles[0].getSquare() > s) {
				return Game.ENEMIES_WIN_BY_SQUARE;
			}
			var square:Number = 0;
			for (var i:int = 0; i < _circles.length; i++ ){
				if (_circles[i] is PlayerCircle) continue;
				var circle:BaseCircle = _circles[i];
				square += circle.getSquare();
				if (square > s) {
					return Game.NO_WIN_BY_SQUARE;
				}
			}
			//размер игрока больше, чем все шары вместе взятые
			if (square < s) return Game.PLAYER_WIN_BY_SQUARE;
			return Game.NO_WIN_BY_SQUARE;
		}
		
		public function start():void {
			_circles = new Vector.<BaseCircle>;
			_rectangles = new Vector.<Rectangle>;
			_player = new PlayerCircle();
			_timer = new Timer(10);
			_rootContainer = new Sprite();
			_textContainer = new Sprite();
			
			_grid = new CollisionGrid();
			_gameEnd = false;
			addChild(_rootContainer);
			addChild(_textContainer);
			drawField();				
					
			_player.radius = _userRadius;
			_player.color = _userColor;
			
			createAndPlacingCircles();	
			
			addEventListener(MouseEvent.MOUSE_DOWN, onMouseDown);
			addEventListener(MouseEvent.MOUSE_UP, onMouseUp);
			
			_timer.addEventListener(TimerEvent.TIMER, onTimer);
			_timer.start();
			addEventListener(Event.ENTER_FRAME, onEnterFrame);	
			_grid.addEventListener(ApplicationEvents.HIT_CIRCLES, onHitCircles);			
		}
		
		public function stop():void {
			removeEventListener(MouseEvent.MOUSE_DOWN, onMouseDown);
			removeEventListener(MouseEvent.MOUSE_UP, onMouseUp);
			_grid.removeEventListener(ApplicationEvents.HIT_CIRCLES, onHitCircles);
			_timer.stop();
			_timer.removeEventListener(TimerEvent.TIMER, onTimer);
			removeEventListener(Event.ENTER_FRAME, onEnterFrame);
			removeChild(_rootContainer);
			
			_circles = null;
			_rectangles = null;
			_player = null;
			_timer = null;
			_grid = null;
			_rootContainer = null;
			
		}
		
	}

}