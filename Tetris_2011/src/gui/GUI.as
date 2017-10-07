package gui
{
	import flash.display.Sprite;
	import flash.filters.GlowFilter;
	import flash.media.Video;
	import flash.text.TextField;
	import logic.TetrisShape;
	import logic.User;
	
	/**
	 * ...
	 * @author 
	 */
	public class GUI extends Sprite 
	{
		private var _fieldView:Sprite = new Sprite();
		private var _shapeView:Sprite = new Sprite();		
		private var _gameFieldBorder:Sprite;
		
		private var _pauseWindow : Sprite = new Sprite();
		
		private var _expText:TextField;		
		private var _levelText:TextField;
		
		private var _userInfoView:UserInfoView;
		private var _nextShapeView:NextShapeView;
		private var _window:Window;
		
		public function GUI($width:int, $height:int) 
		{			
			_gameFieldBorder = View.createBorder($width, $height);			
			addChild(_gameFieldBorder);			
			
			_nextShapeView = new NextShapeView();
			_nextShapeView.x = _gameFieldBorder.x + _gameFieldBorder.width;
			_nextShapeView.y = _gameFieldBorder.y;
			addChild(_nextShapeView);
			
			_userInfoView = new UserInfoView();
			_userInfoView.x = _gameFieldBorder.x + _gameFieldBorder.width;
			_userInfoView.y = 140;
			addChild(_userInfoView);
			
			_window = new Window();
			_window.x = (_gameFieldBorder.width - _window.width) / 2;
			_window.y = (_gameFieldBorder.height - _window.height) / 2;		
		}
		
		public function drawField($arrGlass:Array):void 
		{
			if (_gameFieldBorder.contains(_fieldView)) _gameFieldBorder.removeChild(_fieldView);			
			_fieldView = View.createField($arrGlass);		
			_fieldView.x = (_gameFieldBorder.width - _fieldView.width) / 2;
			_fieldView.y = (_gameFieldBorder.height - _fieldView.height) / 2;
			_gameFieldBorder.addChild(_fieldView);
		}
		
		public function drawShape($shape:TetrisShape):void 
		{
			if (_fieldView.contains(_shapeView)) _fieldView.removeChild(_shapeView);			
			_shapeView = View.createShape($shape);		
			_shapeView.x = $shape.position.y * Config.CELL_SIZE;
			_shapeView.y = $shape.position.x * Config.CELL_SIZE;			
			_fieldView.addChild(_shapeView);
		}
		
		public function drawNextShape($shape:TetrisShape):void 
		{
			_nextShapeView.nextShapeSprite = View.createShape($shape);
		}
		
		public function drawUserInfo($user:User):void 
		{
			_userInfoView.expTextValue = $user.exp.toString() + " (" + $user.expToNextLevel.toString() + ")";			
			_userInfoView.levelTextValue = $user.level.toString();
		}		
				
		public function drawPause($isPause:Boolean):void 
		{
			if ($isPause) {
				_window.titleTextValue = Config.getLocalizationText(Config.LOCAL_TEXT_PAUSE);
				_window.messageTextValue = Config.getLocalizationText(Config.LOCAL_TEXT_UNPAUSE);
				_gameFieldBorder.addChild(_window);
			}else{
				_window.close();			
			}
			
		}
		
		public function drawFinish($isFinish:Boolean):void 
		{
			if ($isFinish) {
				_window.titleTextValue = Config.getLocalizationText(Config.LOCAL_TEXT_LOSE_TITLE);
				_window.messageTextValue = Config.getLocalizationText(Config.LOCAL_TEXT_UNPAUSE);
				_gameFieldBorder.addChild(_window);
			}else{
				_window.close();			
			}
			
		}
	}
	
}