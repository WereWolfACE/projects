package 
{
	import flash.events.TimerEvent;
	import flash.utils.Timer;
	import starling.display.Sprite;
	import starling.events.Event;
	
	
	/**
	 * ...
	 * @author Marshalova Valery
	 */
	public class Main extends Sprite 
	{
		private var _FPSCout:int;
		private var _settingsLoader:SettingsLoader;
		private var game:Game;
			
		public function Main():void 
		{
			if (stage) init();
			else addEventListener(Event.ADDED_TO_STAGE, init);
		}
		
		private function init(e:Event = null):void 
		{
			removeEventListener(Event.ADDED_TO_STAGE, init);
			addEventListener(Event.ENTER_FRAME, onEnterFrame);
			
			// entry point
			_settingsLoader = new SettingsLoader();
			_settingsLoader.addEventListener(SettingsLoader.SETTINGS_LOADED, onSettingsLoaded);
			
			
			
			showFPS();
		}
		
		private function onSettingsLoaded(e:Event):void 
		{			
			initGame(_settingsLoader.settingData);
		}
		
		private function initGame(settings : Object):void 
		{
			var count:int = settings.enemy.count;
			var minRadius:int = settings.enemy.minSize;
			var maxRadius:int = settings.enemy.maxSize;
			var userRadius:int = settings.user.size;
			var rgbUser:Array = settings.user.color;
			var userColor:int = rgbUser[2] << 16 | rgbUser[1] << 8 | rgbUser[0];
			var rgbEnemy1:Array = settings.enemy.color1;
			var rgbEnemy2:Array = settings.enemy.color2;
			var fieldWidth:int = settings.fieldWidth;
			var fieldHeight:int = settings.fieldHeight;
			var resistance:int = settings.resistance;
			
			game = new Game(count, minRadius, maxRadius, userRadius,
									userColor, rgbEnemy1, rgbEnemy2,
									fieldWidth, fieldHeight, resistance);
			addChild(game);
			game.start();
			stage.addEventListener(KeyboardEvent.KEY_DOWN, onKeyDown);
		}
		
		private function onKeyDown(e:KeyboardEvent):void 
		{
			switch(e.charCode) {
				case 110:
					if (e.ctrlKey) {
						game.stop();
						game.start();
					}
					break;
			}
		}
		
		private function onEnterFrame(e:Event):void 
		{
			_FPSCout++; 
		}
		
		private function showFPS():void 
		{
			var _fpsTimer:Timer = new Timer(1000);
			_fpsTimer.addEventListener(TimerEvent.TIMER, onTimerTick);
			_fpsTimer.start();
		}
		
		private function onTimerTick(e:TimerEvent):void 
		{
			_FPSCout = 0;
		}
		
	}
	
}