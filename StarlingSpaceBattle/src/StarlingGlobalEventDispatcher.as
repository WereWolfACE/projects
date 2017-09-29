package  {
	import starling.events.Event;
	import starling.events.EventDispatcher;


    public final class StarlingGlobalEventDispatcher extends EventDispatcher  {
        private static var _instance:StarlingGlobalEventDispatcher = new StarlingGlobalEventDispatcher();  

        public static function get instance():StarlingGlobalEventDispatcher {
            if (_instance == null) {
                _instance = new StarlingGlobalEventDispatcher();
            }
            return _instance;  
        }
        
        public override function dispatchEvent(event:Event):void {
            if (hasEventListener(event.type) || event.bubbles) { // экономим процессорное время
                super.dispatchEvent(event);
            }
        }
    }
}