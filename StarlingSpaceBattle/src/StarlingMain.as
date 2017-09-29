package  
{
	import Constants.GameEvents;
	import flash.system.Capabilities;
	import Preloader.GamePreloader;
	import starling.display.Sprite;
	import starling.display.Stage;
	import starling.events.Event;
	import starling.utils.AssetManager;
	/**
	 * ...
	 * @author werewolf
	 */
	public class StarlingMain extends Sprite
	{
		private var _assets:AssetManager;
		private static var _stage:Stage;
		
		public function StarlingMain() 
		{
			if (stage)
				init();
			else
				addEventListener(Event.ADDED_TO_STAGE, init);
		}
		
		public function init():void
		{
			removeEventListener(Event.ADDED_TO_STAGE, init);	
			_stage = stage;	
			addChild(new GamePreloader());
			loadAssets();
			StarlingGlobalEventDispatcher.instance.addEventListener(GameEvents.PRESS_BUTTON_START, onStart);		
		}
		
		private function onStart(e:Event):void 
		{
			StarlingGlobalEventDispatcher.instance.removeEventListener(GameEvents.PRESS_BUTTON_START, onStart);
			addChild(Game.instance);
			Game.instance.start();
		}
		
		private function loadAssets():void
		{
			_assets = new AssetManager();
			_assets.verbose = Capabilities.isDebugger;
			_assets.enqueue(AssetLibClasses);
			AssetLib.init(_assets, onAssetsLoaded);
		}
		
		private function onAssetsLoaded():void 
		{
			StarlingGlobalEventDispatcher.instance.dispatchEventWith(GameEvents.ASSETS_LOADED);			
		}
		
		static public function get stage():Stage 
		{
			return _stage;
		}

	}

}