package 
{
	import flash.events.Event;
	
	/**
	*
	* @author Marshalova Valery
	*/
	public class ApplicationEvents extends Event
	{
		public static const HIT_CIRCLES:String = "hitCircles";
		private var _params:Object;
		
		public function ApplicationEvents(type:String, param:Object = null, bubbles:Boolean=false, cancelable:Boolean=false)
		{
			_params = param;
			super(type, bubbles, cancelable);
		}

		public function get params():Object 
		{
			return _params;
		}
	}
}