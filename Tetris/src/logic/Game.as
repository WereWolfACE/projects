package logic
{
	import flash.display.Sprite;
	import flash.events.Event;
	import gui.GUI;
	
	/**
	 * ...
	 * @author Marshalova Valeriya
	 */
	public class Game 
	{
		private var _user:User;
		private var _field:GameField;
		
		private var _gui:GUI;
	
		private static var _instance:Game;
		
		public static function getInstance():Game 
		{
			if (!_instance)
				_instance = new Game();
			return _instance;			
		}
		
		public function destroy():void
		{	
			_instance = null;
			if (Main.getInstance().contains(_gui)) 
				Main.getInstance().removeChild(_gui);			
		}
		
		public function Game() 
		{
			newGame();			
		}	
		
		public function newGame():void 
		{			
			_field = new GameField(Config.FIELD_WIDTH, Config.FIELD_HEIGHT);
			_user = new User();						

			_gui = new GUI(_field.width, _field.height);			 
			Main.getInstance().addChild(_gui);
			drawUser();		
		}
		
		public function run():void 
		{
			_field.play();
		}		
		
		public function finishGame(value:Boolean):void 
		{
			_gui.drawFinish(value);					
		}		
		
		/**
		 * time of shape falling (each level accelerated by Config.DECREASE_TIME)
		 * @return time of shape falling
		 */
		public function getShapeFallingTime():int 
		{
			var delay:int = Config.DEFAULT_TIME - ((_user.level - 1) * Config.DECREASE_TIME);
			return delay >= Config.DECREASE_TIME ? delay : Config.DECREASE_TIME;
		}
		
		/**
		 * add experience to user
		 * @param	$collectedRows
		 */
		public function updateUserExp($collectedRows:int):void 
		{
			_user.setExp($collectedRows);
			drawUser();
		}
		
		
		//--------------------------------------------------------------------------
		//
		//  Drawing methods
		//
		//--------------------------------------------------------------------------

		public function drawField($arrGlass:Array):void 
		{
			_gui.drawField($arrGlass);			
		}
		
		public function drawShape($shape:TetrisShape):void 
		{
			_gui.drawShape($shape);			
		}
		
		public function drawNextShape($shape:TetrisShape):void 
		{
			_gui.drawNextShape($shape);			
		}
		
		public function drawUser():void 
		{
			_gui.drawUserInfo(_user);
		}		
		
		public function drawPause($isPause:Boolean):void 
		{
			_gui.drawPause($isPause);
		}		
	}
	
}