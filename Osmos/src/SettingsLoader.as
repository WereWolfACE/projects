package  
{
	import flash.events.Event;
	import flash.events.EventDispatcher;
	import flash.net.URLLoader;
	import flash.net.URLLoaderDataFormat;
	import flash.net.URLRequest;
	
	/**
	 * ...
	 * @author Marshalova Valery
	 */
	public class SettingsLoader extends EventDispatcher 
	{
		public static const SETTINGS_LOADED:String = "settingsLoaded";
		
		private var _urlLoader:URLLoader;
		private var _settingData:Object;
		
		public function SettingsLoader() 
		{
			init();
		}
		
		private function init():void 
		{
			_urlLoader = new URLLoader();
			_urlLoader.dataFormat = URLLoaderDataFormat.TEXT;
			_urlLoader.addEventListener(Event.COMPLETE, onLoadComplete);
			_urlLoader.load(new URLRequest("settings.txt"));
		}
		
		private function onLoadComplete(e:Event):void 
		{
			_settingData = JSON.parse(e.target.data);
			dispatchEvent(new Event(SETTINGS_LOADED));
		}
		
		public function get settingData():Object 
		{
			return _settingData;
		}
		
	}

}