package 
{
	import flash.display.SimpleButton;
	import flash.display.Sprite;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.geom.Point;
	import gui.Button;
	import logic.Game;
	import utils.Loader;
	
	/**
	 * ...
	 * @author Marshalova Valeriya 
	 */
	public class Main extends Sprite 
	{
		private static var _instance:Main;		
		private var _startButton:Button;
		
		public static function getInstance():Main 
		{			
			return _instance;			
		}
		
		public function Main():void 
		{
			if (stage) init();
			else addEventListener(Event.ADDED_TO_STAGE, init);
		}
		
		private function init(e:Event = null):void 
		{
			removeEventListener(Event.ADDED_TO_STAGE, init);
			// entry point						
			_instance = this;
			
			Loader.getInstance().loadShapes(onResourcesLoad);					
		}
		
		/**
		 * start the game
		 * @param	$e
		 */
		private function onResourcesLoad($e:Event):void 
		{
			parseXML(new XML($e.target.data));
			_startButton = new Button(Config.getLocalizationText(Config.LOCAL_TEXT_START), onStartButtonClickHandler);
			_startButton.x = (430 - _startButton.width) / 2;
			_startButton.y = (520 - _startButton.height) / 2;
			addChild(_startButton);			
		}
		
		private function onStartButtonClickHandler($e:MouseEvent) : void
		{			
			_startButton.destroy();
			Game.getInstance().run();
		}
		
		/**
		 * parse shapes xml and add shapes to array Config.SHAPES
		 * @param	$xml
		 */
		public function parseXML($xml:XML):void 
		{			
			var shapes:XMLList = $xml.children();			
			for(var i:uint = 0; i<shapes.length(); i++)
			{					
				var o:Object = new Object;
				o.color = uint(shapes[i].@color);
				o.points = new Array();
				var points:XMLList = shapes[i].child("Points").children();			
				for(var j:uint = 0; j<points.length(); j++)
				{
					o.points.push(new Point(points[j].@x, points[j].@y));					
				}
				Config.SHAPES.push(o);
			}			
		}		
		
	}
	
}