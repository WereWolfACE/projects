package 
{
	import flash.display.Sprite;
	import flash.display.StageAlign;
	import flash.display.StageScaleMode;
	import flash.events.Event;
	import flash.system.Capabilities;
	import starling.core.Starling;
	import starling.utils.AssetManager;
	
	/**
	 * ...
	 * @author werewolf
	 */
	public class Main extends Sprite 
	{		
		public function Main():void 
		{
			if (stage) init();
			else addEventListener(Event.ADDED_TO_STAGE, init);
		}
		
		private function init(e:Event = null):void 
		{
			removeEventListener(Event.ADDED_TO_STAGE, init);
			// entry point
			
			stage.scaleMode = StageScaleMode.NO_SCALE;
            stage.align = StageAlign.TOP_LEFT;
			
			var starling:Starling = new Starling(StarlingMain, stage);
			starling.start();			
			Starling.current.showStats = true;			
		}		
	}
	
}