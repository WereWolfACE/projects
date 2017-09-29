package utils
{
	import flash.events.Event;
	import flash.events.IOErrorEvent;
	import flash.events.SecurityErrorEvent;
	import flash.net.URLLoader;
	import flash.net.URLRequest;
	
	/**
	 * ...
	 * @author Marshalova Valeriya 
	 */
	public class Loader 
	{
		private static var _instance:Loader;				
		
		public static function getInstance():Loader {
			if (!_instance)
				_instance = new Loader();
			return _instance;			
		}
		
		public function Loader() {
			
		}
		
		/**
		 * load xml file with shapes declaration
		 * @param	$onCompleteHandler
		 */
		public function loadShapes($onCompleteHandler:Function):void {
			var loader:URLLoader = new URLLoader();
			loader.load(new URLRequest(Config.SHAPES_XML_PATH));
			loader.addEventListener(Event.COMPLETE, $onCompleteHandler);
			loader.addEventListener(IOErrorEvent.IO_ERROR, URLLoaderIOErrorHandler, false, 0, true);
            loader.addEventListener(SecurityErrorEvent.SECURITY_ERROR, URLLoaderSecurityErrorHandler, false, 0, true);					
		}		
		
		private function URLLoaderSecurityErrorHandler($e:SecurityErrorEvent):void
		{
			trace("Security error");
		}
		
		private function URLLoaderIOErrorHandler($e:IOErrorEvent):void
		{
			trace("File shapes.xml not found");
		}
	}
	
}