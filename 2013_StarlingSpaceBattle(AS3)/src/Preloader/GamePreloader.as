package Preloader
{
	import Constants.GameEvents;
	import starling.display.Sprite;
	import starling.events.Event;
	
	public class GamePreloader extends Sprite
	{
		private var _clip:LoaderClip;
	
		public function GamePreloader()
		{
			_clip = new LoaderClip();			
			_clip.touchable = false;
			_clip.x = StarlingMain.stage.stageWidth / 2;
			_clip.y = StarlingMain.stage.stageHeight / 2;
			addChild(_clip);		
			
			StarlingGlobalEventDispatcher.instance.addEventListener(GameEvents.ASSETS_LOADED, onAssetsLoaded);
		}

		private function onAssetsLoaded(e:Event):void 
		{
			StarlingGlobalEventDispatcher.instance.removeEventListener(GameEvents.ASSETS_LOADED, onAssetsLoaded);
			_clip.changeToButton();	
			StarlingGlobalEventDispatcher.instance.addEventListener(GameEvents.PRESS_BUTTON_START, destructor);
		}
		
		private function destructor(e:Event):void 
		{
			StarlingGlobalEventDispatcher.instance.removeEventListener(GameEvents.PRESS_BUTTON_START, destructor);
			removeFromParent(true);
		}
		
		
		
	}
}